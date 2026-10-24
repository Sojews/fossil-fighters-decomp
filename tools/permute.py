#!/usr/bin/env python3
"""Randomly rewrites one function's source until mwcc's output for it matches the game's (a permuter).

Usage: python3 tools/permute.py src/foo.cpp SYMBOL "DEFINITION START" [-j JOBS] [-t SECONDS] [--seed N]

SYMBOL is the function's (mangled) name as the game's symbols.txt has it; DEFINITION START is a literal prefix of
the line that starts its definition, e.g. "void MSimpleMenu::Layout(". The whole file is recompiled for every
candidate (with the file's own flags from build.ninja), so the result is exact; only the function's body, and the
bodies of the file's static inline functions it calls (unless --no-helpers), are mutated. Mutations are textual and many don't compile, which costs nothing but time. Candidates scoring at least as
well as the best so far are kept and mutated further; whenever the score improves, the source is written to
build/permute/<SYMBOL>/best-<score>.cpp, and a full match stops the run.

The score is 0 for a match: instructions are aligned by mnemonic, a missing or extra instruction costs 10, and an
aligned instruction whose operands differ costs 3, or 1 when only registers differ (relocated words are
compared by mnemonic only).
"""
import argparse
import difflib
import multiprocessing as mp
import os
import random
import re
import shlex
import shutil
import subprocess
import sys
import tempfile
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
TMP = ROOT / "build/permute/tmp"   # (sources only include through the include paths, so they compile from here)
sys.path.insert(0, str(ROOT / "tools"))
import try_match  # noqa: E402

import capstone  # noqa: E402


# ---- compiling and scoring

def compile_command(src: Path) -> list:
    """The file's compile command (as try_match builds it), without the input and output arguments."""
    ninja = (ROOT / "build.ninja").read_text()
    rel = re.escape(str(src.relative_to(ROOT)))
    cmd = re.search(r"rule cc\n  command = (.*?) -MD -c", ninja).group(1)
    edge = re.search(rf"build \S+: cc(?:_partial|_named)? {rel}(?: \|[^\n]*)?\n(?:  .*\n)*?  extra_cflags = (.*)\n",
                     ninja)
    cmd = cmd.replace("$extra_cflags", edge.group(1) if edge else "")
    cc = re.search(r"^cc = (.*)$", ninja, re.M)
    own = re.search(rf"build \S+: cc(?:_partial|_named)? {rel}(?: \|[^\n]*)?\n(?:  .*\n)*?  cc = (.*)\n", ninja)
    cmd = cmd.replace("$cc", own.group(1) if own else cc.group(1) if cc else "$cc")
    return shlex.split(cmd)


def target(src: Path, symbol: str):
    """(code, relocated offsets, thumb) of the game's function, from its delinked object (relocations included)."""
    unit = str(src.relative_to(ROOT).with_suffix(""))
    for obj in [ROOT / "build/usa/delinks" / (unit + ".o")] + sorted((ROOT / "build/usa/delinks").glob("_dsd_gap*.o")):
        if obj.exists():
            f = our_function(obj, symbol)
            if f is not None:
                syms = try_match.game_symbols().get(symbol, [])
                thumb = syms[0][5] if syms else True
                return f[0], f[1], thumb
    sys.exit(f"{symbol}: not found in the delinked objects")


def our_function(obj: Path, symbol: str):
    """(code, relocated offsets) of the symbol in the object, or None."""
    secs, names, syms, rels, data = try_match.read_elf(obj)
    for name, val, size, typ, shndx in syms:
        if name == symbol and typ == 2 and size:
            code = data(secs[shndx])[val:val + size]
            relocs = {off - val for off in rels[shndx] if val <= off < val + size}
            return code, relocs
    return None


def tokens(code: bytes, thumb: bool, relocs=frozenset()):
    """(mnemonic, full text) per instruction; a relocated word becomes ("reloc", "reloc")."""
    md = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB if thumb else capstone.CS_MODE_ARM)
    out, off = [], 0
    while off < len(code):
        if off in relocs:
            out.append(("reloc", "reloc"))
            off += 4
            continue
        ins = next(md.disasm(code[off:], off, 1), None)
        if ins is None:
            out.append((".data", code[off:off + 2].hex()))
            off += 2
            continue
        text = f"{ins.mnemonic} {ins.op_str}"
        if ins.mnemonic.startswith("b") and ins.mnemonic not in ("bic", "bics"):
            text = ins.mnemonic         # branch targets move with any size change
        out.append((ins.mnemonic, text))
        off += ins.size
    return out


REG = re.compile(r"\b(?:r\d+|ip|lr|sb|sl|fp)\b")


def score(ours, theirs) -> int:
    a, b = [t[0] for t in ours], [t[0] for t in theirs]
    s = 0
    for op, i1, i2, j1, j2 in difflib.SequenceMatcher(None, a, b, autojunk=False).get_opcodes():
        if op == "equal":
            for k in range(i2 - i1):
                x, y = ours[i1 + k][1], theirs[j1 + k][1]
                if x != y:
                    # a register-only difference costs less than a different offset or constant
                    s += 1 if REG.sub("r", x) == REG.sub("r", y) else 3
        else:
            s += 10 * max(i2 - i1, j2 - j1)
    return s


# ---- the function's body

def find_body(text: str, start: str):
    """(open, close) offsets of the braces of the definition that begins with start."""
    i = text.find(start)
    while i != -1:
        j = text.find("{", i)
        semi = text.find(";", i)
        if j != -1 and (semi == -1 or j < semi):
            break
        i = text.find(start, i + 1)
    if i == -1:
        sys.exit(f"definition {start!r} not found")
    depth = 0
    for k in range(j, len(text)):
        c = text[k]
        if c == "{":
            depth += 1
        elif c == "}":
            depth -= 1
            if depth == 0:
                return j, k
    sys.exit("unbalanced braces")


# ---- mutations (each takes the body's lines and returns new lines, or None when it doesn't apply)

IDENT = r"[A-Za-z_]\w*"
OPERAND = r"(?:[A-Za-z_][\w]*(?:(?:\.|->)[A-Za-z_]\w*|\[[^\[\]]+\]|\([^()]*\))*|\d+|0x[0-9A-Fa-f]+)"
TYPES = ["int", "u32", "s32", "u16", "s16", "u8", "s8"]
DECL = re.compile(rf"^(\s*)((?:const )?(?:unsigned |signed )?[A-Za-z_][\w:<>]*(?: \*+| &| )\s*)({IDENT})( = .*;|;)\s*$")


def simple(line):
    s = line.strip()
    return s.endswith(";") and "{" not in s and "}" not in s and not s.startswith(("return", "case", "default",
                                                                                    "break", "continue", "goto"))


def m_swap_adjacent(lines, r):
    idx = [i for i in range(len(lines) - 1) if simple(lines[i]) and simple(lines[i + 1])
           and indent(lines[i]) == indent(lines[i + 1])]
    if not idx:
        return None
    i = r.choice(idx)
    lines = lines[:]
    lines[i], lines[i + 1] = lines[i + 1], lines[i]
    return lines


def indent(line):
    return len(line) - len(line.lstrip())


def m_move_decl(lines, r):
    idx = [i for i, l in enumerate(lines) if DECL.match(l)]
    if not idx:
        return None
    i = r.choice(idx)
    lines = lines[:]
    line = lines.pop(i)
    ind = indent(line)
    # Somewhere else in the same block (same indentation, no block boundary crossed).
    cands = []
    for d in (-1, 1):
        j = i
        while True:
            j += d if d > 0 or j > 0 else 0
            if j < 0 or j > len(lines) or (d < 0 and j == 0 and indent(lines[0]) != ind):
                break
            if d < 0 and j >= 0 and j < len(lines) and indent(lines[j]) < ind:
                break
            if d > 0 and j - 1 < len(lines) and j - 1 >= 0 and indent(lines[j - 1]) < ind:
                break
            cands.append(j)
            if len(cands) > 12 or j == 0 or j == len(lines):
                break
    if not cands:
        return None
    lines.insert(r.choice(cands), line)
    return lines


def m_split_decl(lines, r):
    idx = [i for i, l in enumerate(lines) if DECL.match(l) and " = " in l and not l.strip().startswith("const ")]
    if not idx:
        return None
    i = r.choice(idx)
    m = DECL.match(lines[i])
    ind, typ, name, init = m.groups()
    lines = lines[:]
    lines[i:i + 1] = [f"{ind}{typ}{name};", f"{ind}{name}{init}"]
    return lines


def m_join_decl(lines, r):
    out = []
    for i, l in enumerate(lines):
        m = DECL.match(l)
        if m and m.group(4) == ";":
            for j in range(i + 1, min(i + 30, len(lines))):
                if re.match(rf"^\s*{re.escape(m.group(3))} = [^=].*;\s*$", lines[j]) and indent(lines[j]) == indent(l):
                    out.append((i, j))
                    break
    if not out:
        return None
    i, j = r.choice(out)
    m = DECL.match(lines[i])
    rhs = lines[j].strip()[len(m.group(3)):]
    lines = lines[:]
    lines[j] = f"{m.group(1)}{m.group(2)}{m.group(3)}{rhs}"
    del lines[i]
    return lines


def m_const(lines, r):
    idx = [i for i, l in enumerate(lines) if DECL.match(l) and " = " in l]
    if not idx:
        return None
    i = r.choice(idx)
    lines = lines[:]
    l = lines[i]
    s = l.lstrip()
    lines[i] = l[:indent(l)] + (s[6:] if s.startswith("const ") else "const " + s)
    return lines


def m_type(lines, r):
    pat = re.compile(r"\b(" + "|".join(TYPES) + r")\b(?= " + IDENT + r"( = |;|\)|,))")
    idx = [i for i, l in enumerate(lines) if pat.search(l)]
    if not idx:
        return None
    i = r.choice(idx)
    ms = list(pat.finditer(lines[i]))
    m = r.choice(ms)
    new = r.choice([t for t in TYPES[:3] if t != m.group(1)])
    lines = lines[:]
    lines[i] = lines[i][:m.start()] + new + lines[i][m.end():]
    return lines


COMM = re.compile(rf"({OPERAND}) (\+|\*|&|\||\^|==|!=) ({OPERAND})")


def m_commute(lines, r):
    idx = [i for i, l in enumerate(lines) if COMM.search(l) and not l.strip().startswith("//")]
    if not idx:
        return None
    i = r.choice(idx)
    ms = list(COMM.finditer(lines[i]))
    m = r.choice(ms)
    lines = lines[:]
    lines[i] = lines[i][:m.start()] + f"{m.group(3)} {m.group(2)} {m.group(1)}" + lines[i][m.end():]
    return lines


def m_compound(lines, r):
    a = re.compile(rf"^(\s*)({OPERAND}) = \2 ([-+*/&|^]|<<|>>) (.*);\s*$")
    b = re.compile(rf"^(\s*)({OPERAND}) ([-+*/&|^]|<<|>>)= (.*);\s*$")
    idx = [i for i, l in enumerate(lines) if a.match(l) or b.match(l)]
    if not idx:
        return None
    i = r.choice(idx)
    lines = lines[:]
    m = a.match(lines[i])
    if m:
        lines[i] = f"{m.group(1)}{m.group(2)} {m.group(3)}= {m.group(4)};"
    else:
        m = b.match(lines[i])
        lines[i] = f"{m.group(1)}{m.group(2)} = {m.group(2)} {m.group(3)} {m.group(4)};"
    return lines


def m_incr(lines, r):
    pat = re.compile(rf"(\+\+({IDENT})\b|\b({IDENT})\+\+|\b({IDENT}) \+= 1\b)")
    idx = [i for i, l in enumerate(lines) if pat.search(l)]
    if not idx:
        return None
    i = r.choice(idx)
    m = r.choice(list(pat.finditer(lines[i])))
    v = m.group(2) or m.group(3) or m.group(4)
    forms = [f"++{v}", f"{v}++", f"{v} += 1"]
    lines = lines[:]
    lines[i] = lines[i][:m.start()] + r.choice([f for f in forms if f != m.group(0)]) + lines[i][m.end():]
    return lines


def m_block(lines, r):
    """Wraps a run of simple statements in braces (a new scope moves stack slots), or unwraps a bare block."""
    bare = [i for i, l in enumerate(lines) if l.strip() == "{"]
    if bare and r.random() < 0.3:
        i = r.choice(bare)
        depth = 0
        for j in range(i, len(lines)):
            depth += lines[j].count("{") - lines[j].count("}")
            if depth == 0:
                lines = lines[:]
                del lines[j]
                del lines[i]
                return lines
        return None
    idx = [i for i, l in enumerate(lines) if simple(l)]
    if not idx:
        return None
    i = r.choice(idx)
    n = 1
    while i + n < len(lines) and simple(lines[i + n]) and indent(lines[i + n]) == indent(lines[i]) and r.random() < 0.6:
        n += 1
    ind = " " * indent(lines[i])
    return lines[:i] + [ind + "{"] + ["    " + l for l in lines[i:i + n]] + [ind + "}"] + lines[i + n:]


def m_cast(lines, r):
    pat = re.compile(rf"(?<![\w.>\])])({IDENT})(?![\w(\[]|\.|->)")
    idx = [i for i, l in enumerate(lines) if simple(l) and " = " in l]
    if not idx:
        return None
    i = r.choice(idx)
    l = lines[i]
    eq = l.index(" = ") + 3
    ms = [m for m in pat.finditer(l, eq) if not m.group(1).isupper() and m.group(1) not in ("const", "sizeof", "new")]
    if not ms:
        return None
    m = r.choice(ms)
    lines = lines[:]
    lines[i] = l[:m.start()] + f"({r.choice(TYPES[:3])}) {m.group(1)}" + l[m.end():]
    return lines


def m_temp(lines, r):
    """Moves a statement's right-hand side (or a call argument) into a new local of a guessed integer type."""
    pat = re.compile(r"^(\s*)(.*?) = ([^;]+);\s*$")
    idx = [i for i, l in enumerate(lines) if simple(l) and pat.match(l) and not DECL.match(l)]
    if not idx:
        return None
    i = r.choice(idx)
    m = pat.match(lines[i])
    name = f"perm{r.randrange(1000)}"
    typ = r.choice(TYPES[:3])
    lines = lines[:]
    lines[i:i + 1] = [f"{m.group(1)}{typ} {name} = {m.group(3)};", f"{m.group(1)}{m.group(2)} = {name};"]
    return lines


def m_reassoc(lines, r):
    pat = re.compile(rf"({OPERAND}) ([-+]) ({OPERAND}) ([-+]) ({OPERAND})")
    idx = [i for i, l in enumerate(lines) if pat.search(l)]
    if not idx:
        return None
    i = r.choice(idx)
    m = r.choice(list(pat.finditer(lines[i])))
    if m.group(2) == "+" and m.group(4) == "+":
        new = f"{m.group(1)} + ({m.group(3)} + {m.group(5)})"
    elif m.group(2) == "+" and m.group(4) == "-":
        new = f"{m.group(1)} + ({m.group(3)} - {m.group(5)})"
    else:
        return None
    lines = lines[:]
    lines[i] = lines[i][:m.start()] + new + lines[i][m.end():]
    return lines


def m_cond(lines, r):
    pat = re.compile(rf"\((!?)({OPERAND})( != 0| == 0)?\)")
    idx = [i for i, l in enumerate(lines) if l.strip().startswith(("if (", "while (", "} else if (")) and pat.search(l)]
    if not idx:
        return None
    i = r.choice(idx)
    m = r.choice(list(pat.finditer(lines[i])))
    neg = bool(m.group(1)) != (m.group(3) == " == 0")
    v = m.group(2)
    new = r.choice([f"(!{v})", f"({v} == 0)"] if neg else [f"({v})", f"({v} != 0)"])
    if new == m.group(0):
        return None
    lines = lines[:]
    lines[i] = lines[i][:m.start()] + new + lines[i][m.end():]
    return lines


def m_named_ctor(lines, r):
    """Hoists a constructor temporary (MPoint(...), MRect(...), MFx32(...)) into a named local before its statement."""
    pat = re.compile(r"\b(MPoint|MRect|MFx32|MFx32Point)\(([^()]*(?:\([^()]*\)[^()]*)*)\)")
    idx = [i for i, l in enumerate(lines) if (simple(l) or l.strip().startswith("return ")) and l.strip().endswith(";")
           and pat.search(l) and not DECL.match(l)]
    if not idx:
        return None
    i = r.choice(idx)
    m = r.choice(list(pat.finditer(lines[i])))
    name = f"perm{r.randrange(1000)}"
    ind = " " * indent(lines[i])
    decl = f"{ind}{m.group(1)} {name}({m.group(2)});" if m.group(2) else f"{ind}{m.group(1)} {name};"
    lines = lines[:]
    lines[i:i + 1] = [decl, lines[i][:m.start()] + name + lines[i][m.end():]]
    return lines


def m_postinc(lines, r):
    """*p++ = e;  <->  *p = e; p++;"""
    a = re.compile(rf"^(\s*)\*({IDENT})(\+\+|--) = (.*);\s*$")
    idx = [i for i, l in enumerate(lines) if a.match(l)]
    pairs = [i for i in range(len(lines) - 1)
             if re.match(rf"^\s*\*({IDENT}) = .*;\s*$", lines[i])
             and re.match(rf"^\s*{re.escape(re.match(r'^\s*\*(\w+)', lines[i]).group(1))}(\+\+|--);\s*$", lines[i + 1])]
    if not idx and not pairs:
        return None
    lines = lines[:]
    if idx and (not pairs or r.random() < 0.5):
        i = r.choice(idx)
        m = a.match(lines[i])
        lines[i:i + 1] = [f"{m.group(1)}*{m.group(2)} = {m.group(4)};", f"{m.group(1)}{m.group(2)}{m.group(3)};"]
    else:
        i = r.choice(pairs)
        v = re.match(r"^\s*\*(\w+) = (.*);\s*$", lines[i])
        op = lines[i + 1].strip()[len(v.group(1)):-1]
        lines[i:i + 2] = [f"{' ' * indent(lines[i])}*{v.group(1)}{op} = {v.group(2)};"]
    return lines


def m_decl_to_top(lines, r):
    """Declares a block-local variable at the top of the function (assigned where it was declared)."""
    idx = [i for i, l in enumerate(lines) if DECL.match(l) and indent(l) > min(indent(x) for x in lines if x.strip())]
    if not idx:
        return None
    i = r.choice(idx)
    m = DECL.match(lines[i])
    ind, typ, name, init = m.groups()
    if typ.startswith("const ") or "&" in typ:
        return None
    top = next(k for k, x in enumerate(lines) if x.strip())
    lines = lines[:]
    lines[i] = f"{ind}{name}{init}" if init != ";" else None
    lines = [x for x in lines if x is not None]
    lines.insert(top, " " * indent(lines[top]) + f"{typ}{name};")
    return lines


def m_decl_earlier(lines, r):
    """Splits a declaration and moves the bare declaration up past other declarations of the same block."""
    idx = [i for i, l in enumerate(lines) if DECL.match(l) and not l.strip().startswith("const ")
           and "&" not in DECL.match(l).group(2)]
    if not idx:
        return None
    i = r.choice(idx)
    m = DECL.match(lines[i])
    ind, typ, name, init = m.groups()
    ups = []
    j = i - 1
    while j >= 0 and indent(lines[j]) >= len(ind) and len(ups) < 8:
        if indent(lines[j]) == len(ind):
            ups.append(j)
        j -= 1
    if not ups:
        return None
    k = r.choice(ups)
    lines = lines[:]
    if init == ";":
        del lines[i]
    else:
        lines[i] = f"{ind}{name}{init}"
    lines.insert(k, f"{ind}{typ}{name};")
    return lines


def block_end(lines, i):
    """The index of the line closing the block opened on line i (which ends with '{')."""
    depth = 0
    for j in range(i, len(lines)):
        for ch in lines[j]:
            if ch == "{":
                depth += 1
            elif ch == "}":
                depth -= 1
                if depth == 0:
                    return j if j > i else None
    return None


def m_if_swap(lines, r):
    """if (c) { A } else { B }  ->  if (!(c)) { B } else { A }"""
    cands = []
    for i, l in enumerate(lines):
        m = re.match(r"^(\s*)if \((.*)\) \{\s*$", l)
        if not m:
            continue
        j = block_end(lines, i)
        if j is None or lines[j].strip() != "} else {":
            continue
        k = block_end([lines[j].replace("}", "", 1)] + lines[j + 1:], 0)
        k = None if k is None else k + j
        if k is None or lines[k].strip() != "}":
            continue
        cands.append((i, j, k, m.group(1), m.group(2)))
    if not cands:
        return None
    i, j, k, ind, cond = r.choice(cands)
    c = cond[2:-1] if cond.startswith("!(") and cond.endswith(")") else f"!({cond})"
    return lines[:i] + [f"{ind}if ({c}) {{"] + lines[j + 1:k] + [f"{ind}}} else {{"] + lines[i + 1:j] + [f"{ind}}}"] + \
        lines[k + 1:]


def m_arg_hoist(lines, r):
    """Moves one argument of a call (not a plain name or literal) into a local declared just before the statement."""
    pat = re.compile(r"\w\(([^()]*(?:\([^()]*\)[^()]*)*)\)")
    cands = []
    for i, l in enumerate(lines):
        if not (simple(l) or l.strip().startswith("return ")) or not l.strip().endswith(";"):
            continue
        for m in pat.finditer(l):
            args, depth, cur, pos = [], 0, "", m.start(1)
            for ch_i, ch in enumerate(m.group(1)):
                if ch == "," and depth == 0:
                    args.append((pos, cur))
                    pos = m.start(1) + ch_i + 1
                    cur = ""
                    continue
                depth += ch in "([" and 1 or ch in ")]" and -1 or 0
                cur += ch
            args.append((pos, cur))
            for pos, a in args:
                t = a.strip()
                if t and not re.fullmatch(r"[A-Za-z_]\w*|-?\d+|0x[0-9A-Fa-f]+|NULL|TRUE|FALSE", t) and '"' not in t:
                    cands.append((i, pos + len(a) - len(a.lstrip()), t))
    if not cands:
        return None
    i, pos, t = r.choice(cands)
    name = f"perm{r.randrange(1000)}"
    typ = r.choice(TYPES[:3])
    l = lines[i]
    lines = lines[:]
    lines[i:i + 1] = [" " * indent(l) + f"{typ} {name} = {t};", l[:pos] + name + l[pos + len(t):]]
    return lines


def m_ptr_index(lines, r):
    """In a for loop over i, *p++ = e; (with p = base before the loop)  ->  base[i] = e;  (and back is left to drift)"""
    for _ in range(4):
        fors = [i for i, l in enumerate(lines) if re.match(rf"^\s*for \((?:\w+ )?({IDENT}) = 0; \1 < ", l)]
        if not fors:
            return None
        f = r.choice(fors)
        var = re.match(rf"^\s*for \((?:\w+ )?({IDENT})", lines[f]).group(1)
        end = block_end(lines, f)
        if end is None:
            continue
        for k in range(f + 1, end):
            m = re.match(rf"^(\s*)\*({IDENT})\+\+ = (.*);\s*$", lines[k])
            if not m:
                continue
            ptr = m.group(2)
            init = [j for j in range(f) if re.match(rf"^\s*(?:[\w:<>]+ \*+)?{ptr} = ({IDENT});\s*$", lines[j])]
            if not init:
                continue
            base = re.match(rf"^\s*(?:[\w:<>]+ \*+)?{ptr} = ({IDENT});", lines[init[-1]]).group(1)
            lines = lines[:]
            lines[k] = f"{m.group(1)}{base}[{var}] = {m.group(3)};"
            return lines
    return None


MUTATIONS = [(m_swap_adjacent, 3), (m_move_decl, 4), (m_split_decl, 2), (m_join_decl, 2), (m_const, 1),
             (m_type, 2), (m_commute, 3), (m_compound, 1), (m_incr, 1), (m_block, 2), (m_cast, 1), (m_temp, 2),
             (m_reassoc, 1), (m_cond, 1), (m_named_ctor, 2), (m_postinc, 1),
             (m_decl_to_top, 2), (m_decl_earlier, 3), (m_if_swap, 1), (m_arg_hoist, 2), (m_ptr_index, 1)]


def mutate(body: str, r: random.Random) -> str:
    lines = body.split("\n")
    for _ in range(r.choice([1, 1, 1, 2, 2, 3])):
        for _ in range(20):
            f = r.choices([m for m, w in MUTATIONS], [w for m, w in MUTATIONS])[0]
            new = f(lines, r)
            if new is not None and new != lines:
                lines = new
                break
    return "\n".join(lines)


# ---- the search

SEP = "\n//@@permute-part@@\n"   # between the bodies (the function's own first, then its helpers')


def helper_starts(text: str, body: str) -> list:
    """Definition starts of the file's static inline functions that the body calls (their bodies get mutated too)."""
    out = []
    for name in sorted(set(re.findall(r"\b([A-Za-z_]\w*)\(", body))):
        m = re.search(rf"^static inline [^;{{}}\n]*?\b{re.escape(name)}\(", text, re.M)
        if m:
            out.append(m.group(0))
    return out


def spans(text: str, starts: list) -> list:
    return sorted(find_body(text, s) for s in starts)


def split(text: str, starts: list):
    """(frame, parts): the text around the bodies and the bodies, in the starts' order."""
    sp = spans(text, starts)
    order = [sorted(sp).index(find_body(text, s)) for s in starts]
    frame, pos, parts = [], 0, [None] * len(sp)
    for k, (o, c) in enumerate(sp):
        frame.append(text[pos:o + 1])
        parts[order.index(k)] = text[o + 1:c]
        pos = c
    frame.append(text[pos:])
    return frame, parts, order


def assemble(frame, order, body: str) -> str:
    parts = body.split(SEP)
    out = frame[0]
    for k in range(len(frame) - 1):
        out += parts[order.index(k)] + frame[k + 1]
    return out


def mutate_parts(body: str, r: random.Random) -> str:
    parts = body.split(SEP)
    k = 0 if len(parts) == 1 or r.random() < 0.7 else r.randrange(1, len(parts))
    parts[k] = mutate(parts[k], r)
    return SEP.join(parts)


def worker(args):
    wid, src, symbol, starts, cmd, theirs_tokens, thumb, deadline, seed, best_q, shared = args
    r = random.Random(seed)
    frame, _, order = split(src.read_text(), starts)
    tmp = Path(tempfile.mkdtemp(prefix=f"perm{wid}_", dir=TMP))
    path = tmp / src.name
    pool = [(shared["score"], shared["body"])]
    tries = 0
    try:
        while time.time() < deadline and shared["score"] > 0:
            if r.random() < 0.15:           # follow the global best now and then
                pool.append((shared["score"], shared["body"]))
            base = r.choice(sorted(pool)[:4])
            body = mutate_parts(base[1], r)
            path.write_text(assemble(frame, order, body))
            obj = tmp / (src.stem + ".o")
            if obj.exists():
                obj.unlink()
            subprocess.run(cmd + ["-c", str(path), "-o", str(tmp) + "/"], cwd=ROOT, capture_output=True)
            tries += 1
            if not obj.exists():
                continue
            f = our_function(obj, symbol)
            if f is None:
                continue
            s = score(tokens(f[0], thumb, frozenset(f[1])), theirs_tokens)
            if s <= base[0]:
                pool.append((s, body))
                pool = sorted(pool)[:8]
            if s < shared["score"]:
                best_q.put((s, body, wid, tries))
    finally:
        shutil.rmtree(tmp, ignore_errors=True)
    return tries


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("src")
    ap.add_argument("symbol")
    ap.add_argument("start")
    ap.add_argument("-j", type=int, default=max(1, os.cpu_count() - 2))
    ap.add_argument("-t", type=float, default=600, help="seconds")
    ap.add_argument("--seed", type=int, default=None)
    ap.add_argument("--resume", action="store_true", help="start from the best candidate of an earlier run")
    ap.add_argument("--no-helpers", action="store_true", help="don't mutate the static inline helpers it calls")
    a = ap.parse_args()
    src = (ROOT / a.src).resolve()
    text = src.read_text()
    o, c = find_body(text, a.start)
    starts = [a.start] + ([] if a.no_helpers else
                          [h for h in helper_starts(text, text[o + 1:c]) if find_body(text, h) != (o, c)])
    frame, parts, order = split(text, starts)
    body = SEP.join(parts)
    out = ROOT / "build/permute" / a.symbol
    if a.resume:
        bests = sorted(out.glob("best-*.cpp"), key=lambda p: int(p.stem[5:]))
        if bests:
            try:
                _, prev_parts, _ = split(bests[0].read_text(), starts)
                body = SEP.join(prev_parts)
                print(f"resuming from {bests[0].name}", flush=True)
            except SystemExit:
                print(f"can't resume from {bests[0].name}", flush=True)
    if len(starts) > 1:
        print("also mutating:", ", ".join(starts[1:]), flush=True)
    cmd = compile_command(src)
    theirs, trel, thumb = target(src, a.symbol)
    theirs_tokens = tokens(theirs, thumb, frozenset(trel))
    TMP.mkdir(parents=True, exist_ok=True)
    out.mkdir(parents=True, exist_ok=True)

    with tempfile.TemporaryDirectory(dir=TMP) as tmp:
        p = Path(tmp) / src.name
        p.write_text(assemble(frame, order, body))
        subprocess.run(cmd + ["-c", str(p), "-o", tmp + "/"], cwd=ROOT, capture_output=True)
        f = our_function(Path(tmp) / (src.stem + ".o"), a.symbol)
        if f is None:
            sys.exit("the file doesn't compile or doesn't define the symbol")
        base = score(tokens(f[0], thumb, frozenset(f[1])), theirs_tokens)
    print(f"base score {base}", flush=True)
    if base == 0:
        return
    mgr = mp.Manager()
    shared = mgr.dict(score=base, body=body)
    q = mgr.Queue()
    deadline = time.time() + a.t
    seed = a.seed if a.seed is not None else random.randrange(1 << 30)
    jobs = [(i, src, a.symbol, starts, cmd, theirs_tokens, thumb, deadline, seed + i, q, shared) for i in range(a.j)]
    with mp.Pool(a.j) as pool:
        res = pool.map_async(worker, jobs)
        t0 = time.time()
        while not res.ready():
            try:
                s, b, wid, tries = q.get(timeout=1)
            except Exception:
                continue
            if s < shared["score"]:
                shared["score"], shared["body"] = s, b
                (out / f"best-{s}.cpp").write_text(assemble(frame, order, b))
                print(f"[{time.time() - t0:6.0f}s] score {s} (worker {wid}, try {tries}) -> {out}/best-{s}.cpp",
                      flush=True)
        total = sum(res.get())
    print(f"done: best {shared['score']} after {total} compiles", flush=True)


if __name__ == "__main__":
    main()
