#!/usr/bin/env python3
"""Finds the order to declare a file's statics in so mwcc lays them out as the game has them.

Usage: python3 tools/static_order.py SRC.cpp NAME... [--apply] [--search]

mwcc's layout of a section's statics is a fixed permutation of their declaration order that depends only on how
many there are and of which kinds (sizes), not on their names. The statics must be declared one per line between
`// static_order: begin` and `// static_order: end` in SRC; NAME... gives the layout wanted for one section, first
to last. This compiles the file with the statics of one kind kept in place and the others moved through the
positions, reads the permutation from the object, and prints (or with --apply writes) a declaration order that
gives the layout. When that fails (the permutation also depends on the rest of the file, as with pointers to
string literals), --search hill-climbs the order instead, compiling once per candidate move.
"""
import re
import subprocess
import sys
from itertools import permutations
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
from anon_names import Elf  # noqa: E402

BEGIN, END = "// static_order: begin", "// static_order: end"


def compile_cmd(src: Path, real: Path, out: Path):
    ninja = (ROOT / "build.ninja").read_text()
    cmd = re.search(r"rule cc\n  command = (.*?) -MD -c", ninja).group(1)
    m = re.search(r"build \S+: \w+ " + re.escape(str(real.relative_to(ROOT) if real.is_absolute() else real))
                  + r"[^\n]*\n(?:  .*\n)*?  extra_cflags = ([^\n]*)", ninja)
    cmd = cmd.replace("$extra_cflags", m.group(1) if m else "")
    return cmd.split() + ["-c", str(src), "-o", str(out.parent)]


def layout(text: str, src: Path, names):
    tmp = src.with_name("_static_order_tmp" + src.suffix)
    tmp.write_text(text)
    out = ROOT / "build/static_order"
    out.mkdir(parents=True, exist_ok=True)
    obj = out / tmp.with_suffix(".o").name
    if obj.exists():
        obj.unlink()
    r = subprocess.run(compile_cmd(tmp, src, obj), cwd=ROOT, capture_output=True, text=True)
    if not obj.exists():
        sys.exit("compile failed:\n" + r.stdout + r.stderr)
    tmp.unlink()
    elf = Elf(str(obj))
    found = {s["name"]: (s["shndx"], s["value"]) for s in elf.symbols if s["name"] in names}
    return [n for n, _ in sorted(found.items(), key=lambda kv: kv[1])]


def main():
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    src, want = Path(args[0]), args[1:]
    text = src.read_text()
    head, rest = text.split(BEGIN + "\n")
    block, tail = rest.split(END)
    lines = [l for l in block.splitlines() if l.strip()]
    decl = {}
    for l in lines:
        m = re.search(r"(\w+)\s*(?:\[[^\]]*\])?\s*(?:=|\(|;)", l)
        decl[m.group(1)] = l
    others = [n for n in decl if n not in want]

    def text_for(order):
        return head + BEGIN + "\n" + "\n".join(decl[n] for n in order) + "\n" + END + tail

    # The kind of a static is its declaration minus its name and initializer.
    def kind(n):
        return re.sub(r"\b" + n + r"\b.*", "", decl[n]).strip()
    kinds = {}
    for n in want:
        kinds.setdefault(kind(n), []).append(n)
    common = max(kinds, key=lambda k: len(kinds[k]))
    special = [n for n in want if kind(n) != common]
    N = len(want)
    for positions in permutations(range(N), len(special)) if special else [()]:
        for perm_special in [special]:
            seq = [None] * N
            for p, n in zip(positions, perm_special):
                seq[p] = n
            fill = iter(kinds[common])
            seq = [s if s is not None else next(fill) for s in seq]
            lay = layout(text_for(seq + others), src, set(want))
            if len(lay) != N:
                continue
            # where each declaration position lands
            pos_of = {n: i for i, n in enumerate(lay)}
            dest = [pos_of[n] for n in seq]
            # the special statics must land where the game has them
            if any(want[dest[seq.index(n)]] != n for n in special):
                continue
            order = [want[d] for d in dest]
            check = layout(text_for(order + others), src, set(want))
            if check == want:
                print("\n".join(order))
                if "--apply" in sys.argv:
                    src.write_text(text_for(order + others))
                return
    print("no order found", file=sys.stderr)
    sys.exit(1)


if __name__ == "__main__" and "--search" not in sys.argv:
    main()


def search(src: Path, want, apply: bool):
    """Hill-climbs the declaration order: moves the static that belongs at the first wrong layout position to
    whichever declaration position gives the most statics in their place, until none improves."""
    text = src.read_text()
    head, rest = text.split(BEGIN + "\n")
    block, tail = rest.split(END)
    lines = [l for l in block.splitlines() if l.strip()]
    decl = {}
    for l in lines:
        m = re.search(r"(\w+)\s*(?:\[[^\]]*\])?\s*(?:=|\(|;)", l)
        decl[m.group(1)] = l
    order = [n for n in decl if n in want]
    others = [n for n in decl if n not in want]

    def text_for(o):
        return head + BEGIN + "\n" + "\n".join(decl[n] for n in o + others) + "\n" + END + tail

    def score(o):
        lay = layout(text_for(o), src, set(want))
        return sum(a == b for a, b in zip(lay, want)), lay

    best, lay = score(order)
    print("start", best, file=sys.stderr)
    improved = True
    while improved and best < len(want):
        improved = False
        for j, name in enumerate(want):
            if lay[j] == name:
                continue
            i = order.index(name)
            cands = []
            for k in range(len(order)):
                if k == i:
                    continue
                o = order[:i] + order[i + 1:]
                o.insert(k, name)
                s, l = score(o)
                cands.append((s, o, l))
            s, o, l = max(cands, key=lambda c: c[0])
            if s > best:
                best, order, lay = s, o, l
                improved = True
                print("moved", name, "->", best, file=sys.stderr)
                if apply:
                    src.write_text(text_for(order))
    print("final", best, "of", len(want), file=sys.stderr)
    return order


if __name__ == "__main__" and "--search" in sys.argv:
    a = [x for x in sys.argv[1:] if not x.startswith("--")]
    search(Path(a[0]), a[1:], "--apply" in sys.argv)
