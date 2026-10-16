#!/usr/bin/env python3
"""Names destructors from the game's RTTI: type-name string -> typeinfo -> vtable -> D1/D0 entries.

Usage: python3 tools/rtti_names.py [--apply]
Prints one line per class (vtable, base class, destructors). With --apply, renames the destructors in
symbols.txt to their mangled names and names the type-name strings (_ZTS), typeinfos (_ZTI) and vtables (_ZTV).
A vtable symbol starts at its offset-to-top word, 8 bytes before the address the code loads, as mwcc emits it. A vtable's first two entries are taken as the complete (D1) and
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
    data_names = {}   # addr -> mangled name
    for ti, name in typeinfo.items():
        data_names[ti] = f"_ZTI{name}"
        data_names[word(ti + 4)] = f"_ZTS{name}"
    for ti, name in sorted(typeinfo.items(), key=lambda kv: kv[1]):
        for ref in words.get(ti, []):
            vt = ref + 4
            if word(ref - 4) != 0:
                continue
            data_names[ref - 4] = f"_ZTV{name}"
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
        lines, m = apply_data_names(lines, data_names, seen)
        syms_path.write_text("\n".join(lines) + "\n")
        print(f"# renamed {n} functions, named {m} data symbols", file=sys.stderr)


def rewrite_vtable_relocs(entries: set[int]) -> None:
    """Points relocations to a vtable's entries at the vtable symbol plus 8."""
    for path in CFG.rglob("relocs.txt"):
        text = path.read_text()
        def sub(m):
            to = int(m.group(1), 16)
            return f"to:{to - 8:#010x} add:0x8 module:main" if to in entries else m.group(0)
        new = re.sub(r"to:(0x[0-9a-f]+) module:main", sub, text)
        if new != text:
            path.write_text(new)


def apply_data_names(lines: list[str], data_names: dict[int, str], seen: set[str]) -> tuple[list[str], int]:
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
            if i is not None and lines[i].startswith("data_"):
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
    rewrite_vtable_relocs({int(re.search(r"addr:(0x[0-9a-f]+)", lines[i]).group(1), 16) for i in drop})
    lines = [l for i, l in enumerate(lines) if i not in drop]
    lines.sort(key=lambda l: int(re.search(r"addr:(0x[0-9a-f]+)", l).group(1), 16))
    return lines, n


if __name__ == "__main__":
    main()
