#!/usr/bin/env python3
"""Names destructors from the game's RTTI: type-name string -> typeinfo -> vtable -> D1/D0 entries.

Usage: python3 tools/rtti_names.py [--apply]
Prints one line per class (vtable, base class, destructors). With --apply, renames the destructors in
symbols.txt to their mangled names. A vtable's first two entries are taken as the complete (D1) and
deleting (D0) destructors only if the D0 candidate calls operator delete and the D1 candidate doesn't.
"""
import re
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CFG = ROOT / "config/usa/arm9"
BIN = (ROOT / "extract/usa/arm9/arm9.bin").read_bytes()
BASE = 0x02000000
OP_DELETE = 0x02052e48


def word(addr: int) -> int:
    return struct.unpack_from("<I", BIN, addr - BASE)[0]


def in_main(addr: int) -> bool:
    return BASE <= addr < BASE + len(BIN) - 3


def thumb_calls(start: int, size: int) -> set[int]:
    """Targets of Thumb BL instructions in [start, start+size)."""
    out = set()
    off = start - BASE
    for i in range(off, off + size - 2, 2):
        hi, lo = struct.unpack_from("<HH", BIN, i)
        if hi >> 11 == 0b11110 and lo >> 11 in (0b11111, 0b11101):
            imm = ((hi & 0x7FF) << 12) | ((lo & 0x7FF) << 1)
            if imm & 0x400000:
                imm -= 0x800000
            target = BASE + i + 4 + imm
            if lo >> 11 == 0b11101:
                target &= ~3
            out.add(target)
    return out


def main() -> None:
    syms_path = CFG / "symbols.txt"
    lines = syms_path.read_text().splitlines()
    funcs = {}
    for l in lines:
        m = re.match(r"^(\S+) kind:function\((\w+),size=(0x[0-9a-f]+)\) addr:(0x[0-9a-f]+)", l)
        if m:
            funcs[int(m.group(4), 16)] = (m.group(1), m.group(2), int(m.group(3), 16))

    names = {}
    for m in re.finditer(rb"(?<![\x20-\x7e])(\d{1,2})([A-Za-z_][A-Za-z0-9_]{2,})\x00", BIN):
        if len(m.group(2)) == int(m.group(1)):
            names[BASE + m.start()] = (m.group(1) + m.group(2)).decode()

    words = {}
    for off in range(0, len(BIN) - 3, 4):
        words.setdefault(struct.unpack_from("<I", BIN, off)[0], []).append(BASE + off)

    typeinfo = {}   # typeinfo addr -> mangled class name
    for name_addr, name in names.items():
        for ref in words.get(name_addr, []):
            ti = ref - 4
            if in_main(word(ti)):
                typeinfo[ti] = name
    base_of = {}
    for ti, name in typeinfo.items():
        b = word(ti + 8)
        if b in typeinfo:
            base_of[name] = typeinfo[b]

    renames = {}
    for ti, name in sorted(typeinfo.items(), key=lambda kv: kv[1]):
        for ref in words.get(ti, []):
            vt = ref + 4
            if word(ref - 4) != 0:
                continue
            d1, d0 = word(vt) & ~1, word(vt + 4) & ~1
            if d1 not in funcs or d0 not in funcs:
                continue
            # mwcc inlines D1's body into D0, so D0 is recognised by calling operator delete where D1 doesn't.
            ok = (funcs[d0][1] == "thumb" and funcs[d1][1] == "thumb" and d0 != d1
                  and OP_DELETE in thumb_calls(d0, funcs[d0][2]) and OP_DELETE not in thumb_calls(d1, funcs[d1][2]))
            base = base_of.get(name, "")
            print(f"{name:40} vtable {vt:#010x} base {base:30} D1 {d1:#010x} D0 {d0:#010x} {'ok' if ok else '?'}")
            if ok:
                for addr, kind in ((d1, "D1"), (d0, "D0")):
                    cur = funcs[addr][0]
                    if cur.startswith("func_"):
                        renames[cur] = f"_ZN{name}{kind}Ev"

    print(f"# classes with typeinfo: {len(typeinfo)}, destructors to name: {len(renames)}", file=sys.stderr)
    if "--apply" in sys.argv:
        seen = {l.split(" ", 1)[0] for l in lines}
        n = 0
        for i, l in enumerate(lines):
            tok = l.split(" ", 1)[0]
            new = renames.get(tok)
            if new and new not in seen:
                lines[i] = new + l[len(tok):]
                seen.add(new)
                n += 1
        syms_path.write_text("\n".join(lines) + "\n")
        print(f"# renamed {n}", file=sys.stderr)


if __name__ == "__main__":
    main()
