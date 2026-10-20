#!/usr/bin/env python3
"""Names destructors and RTTI data from the game's RTTI: type-name string -> typeinfo -> vtable -> D1/D0 entries.

Usage: python3 tools/rtti_names.py [--apply] [--modules main,ov001,...]
Prints one line per class (module, vtable, base class, destructors) for the main program and each overlay. With
--apply, renames the destructors in symbols.txt to their mangled names and names the type-name strings (_ZTS),
typeinfos (_ZTI) and vtables (_ZTV). A vtable symbol starts at its offset-to-top word, 8 bytes before the address
the code loads, as mwcc emits it. A vtable's first two entries are taken as the complete (D1) and deleting (D0)
destructors only if the D0 candidate calls operator delete and the D1 candidate doesn't.
--modules limits the renaming to the given modules' symbols.txt and relocs.txt; a vtable's entry symbol that
relocations in another module still point at is kept.
"""
import re
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CFG = ROOT / "config/usa/arm9"
EX = ROOT / "extract/usa"
OP_DELETE = 0x02052e48
# vtables (+8) of __cxxabiv1::__class_type_info, __si_class_type_info and __vmi_class_type_info
TYPEINFO_VTABLES = {0x020c608c, 0x020c607c, 0x020c606c}
# config directories whose files may be rewritten (all of them unless --modules is given)
WRITABLE = None


class Module:
    def __init__(self, label, binpath, base, cfgdir):
        self.label, self.base, self.cfg = label, base, cfgdir
        self.bin = binpath.read_bytes()
        self.words = {}
        for off in range(0, len(self.bin) - 3, 4):
            self.words.setdefault(struct.unpack_from("<I", self.bin, off)[0], []).append(base + off)

    def contains(self, addr):
        return self.base <= addr < self.base + len(self.bin) - 3

    def word(self, addr):
        return struct.unpack_from("<I", self.bin, addr - self.base)[0]

    def thumb_calls(self, start, size):
        """Targets of Thumb BL instructions in [start, start+size)."""
        out = set()
        off = start - self.base
        for i in range(off, off + size - 2, 2):
            hi, lo = struct.unpack_from("<HH", self.bin, i)
            if hi >> 11 == 0b11110 and lo >> 11 in (0b11111, 0b11101):
                imm = ((hi & 0x7FF) << 12) | ((lo & 0x7FF) << 1)
                if imm & 0x400000:
                    imm -= 0x800000
                target = self.base + i + 4 + imm
                if lo >> 11 == 0b11101:
                    target &= ~3
                out.add(target)
        return out


def modules():
    mods = [Module("main", EX / "arm9/arm9.bin", 0x02000000, CFG)]
    text = (EX / "arm9_overlays/overlays.yaml").read_text()
    for i, base in re.findall(r"- id: (\d+)\n\s+base_address: (\d+)", text):
        n = int(i)
        mods.append(Module(f"overlay({n})", EX / f"arm9_overlays/ov{n:03d}.bin", int(base),
                           CFG / f"overlays/ov{n:03d}"))
    return mods


BUILTIN_TYPES = "vwbcahstijlmxyfde"


def _source_name(s: str, i: int):
    """Index after the <length><identifier> at s[i], or None."""
    m = re.match(r"([1-9]\d*)", s[i:])
    if not m:
        return None
    j = i + len(m.group(1))
    n = int(m.group(1))
    name = s[j:j + n]
    if len(name) != n or len(name) < 3 or not re.fullmatch(r"[A-Za-z_]\w*", name):
        return None
    return j + n


def _class_name(s: str, i: int):
    """Index after a class name with optional template arguments (12MBgCallTask0I12DinoMainLoopE), or None."""
    j = _source_name(s, i)
    if j is None or j >= len(s) or s[j] != "I":
        return j
    j += 1
    while j is not None and j < len(s) and s[j] != "E":
        j = _type(s, j)
    return j + 1 if j is not None and j < len(s) else None


def _type(s: str, i: int):
    """Index after one mangled type (a builtin, a pointer/const to a type, or a class name), or None."""
    if s[i] in BUILTIN_TYPES:
        return i + 1
    if s[i] in "PK":
        return _type(s, i + 1) if i + 1 < len(s) else None
    return _class_name(s, i)


def is_mangled_name(s: str) -> bool:
    """Whether s is a whole mangled class name: 5MTask, St9type_info, N10__cxxabiv117__class_type_infoE or a
    template instance such as 12MBgCallTask0I12DinoMainLoopE."""
    if s.startswith("N"):
        if not s.endswith("E"):
            return False
        i, parts = 1, 0
        while i is not None and i < len(s) - 1:
            i = _class_name(s, i)
            parts += 1
        return i == len(s) - 1 and parts > 1
    i = 2 if s.startswith("St") else 0
    return _class_name(s, i) == len(s)


def find_typeinfos(mod):
    """typeinfo address -> mangled class name, for typeinfos in mod whose name string is in mod."""
    names = {}
    for m in re.finditer(rb"(?<![\x20-\x7e])(N|St)?((?:\d{1,2}[A-Za-z_][A-Za-z0-9_]*?)+)(E?)\x00", mod.bin):
        if is_mangled_name(m.group(0)[:-1].decode()):
            names[mod.base + m.start()] = m.group(0)[:-1].decode()
    typeinfo = {}
    for name_addr, name in names.items():
        for ref in mod.words.get(name_addr, []):
            ti = ref - 4
            if mod.contains(ti) and mod.word(ti) in TYPEINFO_VTABLES:
                typeinfo[ti] = name
    return typeinfo


def process(mod, main_typeinfo, apply, used_elsewhere):
    syms_path = mod.cfg / "symbols.txt"
    lines = syms_path.read_text().splitlines()
    funcs = {}
    for l in lines:
        m = re.match(r"^(\S+) kind:function\((\w+),size=(0x[0-9a-f]+)\) addr:(0x[0-9a-f]+)", l)
        if m:
            funcs[int(m.group(4), 16)] = (m.group(1), m.group(2), int(m.group(3), 16))

    typeinfo = find_typeinfos(mod) if mod.label != "main" else main_typeinfo
    known = dict(main_typeinfo)
    known.update(typeinfo)
    base_of = {}
    for ti, name in typeinfo.items():
        b = mod.word(ti + 8)
        if b in known:
            base_of[name] = known[b]

    renames = {}
    data_names = {}   # addr -> mangled name
    for ti, name in typeinfo.items():
        data_names[ti] = f"_ZTI{name}"
        data_names[mod.word(ti + 4)] = f"_ZTS{name}"
    for ti, name in sorted(known.items(), key=lambda kv: kv[1]):
        for ref in mod.words.get(ti, []):
            vt = ref + 4
            if not mod.contains(ref - 4) or mod.word(ref - 4) != 0 or not mod.contains(vt + 4):
                continue
            d1, d0 = mod.word(vt) & ~1, mod.word(vt + 4) & ~1
            if ti not in typeinfo and d1 not in funcs:
                continue    # a base-class pointer in a typeinfo rather than a vtable
            data_names[ref - 4] = f"_ZTV{name}"
            if d1 not in funcs or d0 not in funcs:
                continue
            # mwcc inlines D1's body into D0, so D0 is recognised by calling operator delete where D1 doesn't.
            ok = (funcs[d0][1] == "thumb" and funcs[d1][1] == "thumb" and d0 != d1
                  and OP_DELETE in mod.thumb_calls(d0, funcs[d0][2])
                  and OP_DELETE not in mod.thumb_calls(d1, funcs[d1][2]))
            base = base_of.get(name, "")
            print(f"{mod.label:12} {name:40} vtable {vt:#010x} base {base:30} D1 {d1:#010x} D0 {d0:#010x} "
                  f"{'ok' if ok else '?'}")
            if ok:
                for addr, kind in ((d1, "D1"), (d0, "D0")):
                    cur = funcs[addr][0]
                    if cur.startswith("func_"):
                        inner = name[1:-1] if name.startswith("N") else name
                        renames[cur] = f"_ZN{inner}{kind}Ev"

    print(f"# {mod.label}: classes with typeinfo: {len(typeinfo)}, destructors to name: {len(renames)}",
          file=sys.stderr)
    if apply:
        # A class defined in two overlays has a copy of its RTTI data in each; the later copies get the
        # overlay as a suffix, since symbol names must be unique.
        suffix = re.sub(r"overlay\((\d+)\)", lambda m: f"_ov{int(m.group(1)):03d}", mod.label)
        data_names = {a: (n + suffix if n in used_elsewhere else n) for a, n in data_names.items()}
        renames = {k: v for k, v in renames.items() if v not in used_elsewhere}
        seen = {l.split(" ", 1)[0] for l in lines}
        n = 0
        for i, l in enumerate(lines):
            tok = l.split(" ", 1)[0]
            new = renames.get(tok)
            if new and new not in seen:
                lines[i] = new + l[len(tok):]
                seen.add(new)
                n += 1
        lines, m = apply_data_names(mod, lines, data_names, seen)
        syms_path.write_text("\n".join(lines) + "\n")
        print(f"# {mod.label}: renamed {n} functions, named {m} data symbols", file=sys.stderr)
    return {l.split(" ", 1)[0] for l in syms_path.read_text().splitlines()}


def rewrite_vtable_relocs(mod, entries: set[int]) -> None:
    """Points relocations to a vtable's entries at the vtable symbol plus 8."""
    label = re.escape(mod.label)
    for path in CFG.rglob("relocs.txt"):
        if WRITABLE is not None and path.parent not in WRITABLE:
            continue
        text = path.read_text()

        def sub(m):
            to = int(m.group(1), 16)
            return f"to:{to - 8:#010x} add:0x8 module:{mod.label}" if to in entries else m.group(0)
        new = re.sub(rf"to:(0x[0-9a-f]+) module:{label}", sub, text)
        if new != text:
            path.write_text(new)


def referenced_from_readonly(mod, addr: int) -> bool:
    """Whether a relocs.txt we may not rewrite points at addr in mod."""
    if WRITABLE is None:
        return False
    pat = re.compile(rf"to:{addr:#010x} module:{re.escape(mod.label)}\b")
    return any(pat.search(p.read_text()) for p in CFG.rglob("relocs.txt") if p.parent not in WRITABLE)


def apply_data_names(mod, lines, data_names, seen):
    """Names the RTTI data, adding symbols where there are none. The auto-generated symbol at a vtable's
    entries (8 bytes into it) is dropped, so references to it become _ZTV...+8."""
    by_addr = {}
    for i, l in enumerate(lines):
        m = re.search(r" kind:data\S* addr:(0x[0-9a-f]+)", l)
        if m:
            by_addr[int(m.group(1), 16)] = i
    drop = set()
    for addr, name in data_names.items():
        if name.startswith("_ZTV"):
            i = by_addr.get(addr + 8)
            if i is not None and lines[i].split(" ", 1)[0].startswith("data_") \
                    and not referenced_from_readonly(mod, addr + 8):
                drop.add(i)
    n = 0
    for addr, name in sorted(data_names.items()):
        if name in seen:
            continue
        i = by_addr.get(addr)
        if i is not None:
            tok = lines[i].split(" ", 1)[0]
            if not tok.startswith("data_"):
                continue
            lines[i] = name + lines[i][len(tok):]
        else:
            lines.append(f"{name} kind:data(any) addr:{addr:#010x}")
        seen.add(name)
        n += 1
    rewrite_vtable_relocs(mod, {int(re.search(r"addr:(0x[0-9a-f]+)", lines[i]).group(1), 16) for i in drop})
    lines = [l for i, l in enumerate(lines) if i not in drop]
    lines.sort(key=lambda l: int(re.search(r"addr:(0x[0-9a-f]+)", l).group(1), 16))
    return lines, n


def main() -> None:
    global WRITABLE
    apply = "--apply" in sys.argv
    mods = modules()
    if "--modules" in sys.argv:
        want = sys.argv[sys.argv.index("--modules") + 1].split(",")
        WRITABLE = {CFG if w == "main" else CFG / "overlays" / w for w in want}
    main_typeinfo = find_typeinfos(mods[0])
    names = {}
    for mod in mods:
        names[mod.label] = {l.split(" ", 1)[0] for l in (mod.cfg / "symbols.txt").read_text().splitlines()}
    for mod in mods:
        if WRITABLE is not None and mod.cfg not in WRITABLE:
            continue
        others = set().union(*(v for k, v in names.items() if k != mod.label))
        result = process(mod, main_typeinfo, apply, others)
        if result:
            names[mod.label] = result


if __name__ == "__main__":
    main()
