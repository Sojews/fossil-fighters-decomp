#!/usr/bin/env python3
"""Lists calls whose target differs from the game's: for every function of ours that has a game address, each `bl`
relocation (R_ARM_THM_CALL) is compared with the target of the game's `bl` at the same offset.

Usage: python3 tools/callcheck.py [OBJ ...]        (default: every object under build/usa/src and build/usa/libs)

objdiff's report ignores call-target names (a call to the wrong overload or to our own weak template copy can score
100%), so a gate should also check that this list doesn't grow. Prints one line per mismatch,
"<object> <function>+<offset>: ours <target>  game <target>", and the count on stderr.
"""
import os
import re
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CFG = ROOT / "config/usa/arm9"
OVERLAY_BASES = {range(0, 6): 0x021558a0, range(6, 8): 0x02171b00}


def load_symbols(cfg: Path):
    """(name -> address, address -> first name) of the functions in a module's symbols.txt."""
    n2a, a2n = {}, {}
    for line in open(cfg / "symbols.txt"):
        m = re.match(r"(\S+) kind:function\(\w+,size=0x([0-9a-f]+)(?:,unknown)?\) addr:0x([0-9a-f]+)", line)
        if m:
            a = int(m.group(3), 16)
            n2a[m.group(1)] = a
            a2n.setdefault(a, m.group(1))
    return n2a, a2n


def overlay_calls(cfg: Path):
    """call site -> overlay number, for the calls dsd resolved to one overlay."""
    d = {}
    for line in open(cfg / "relocs.txt"):
        x = re.match(r"from:0x([0-9a-f]+) kind:\w+ to:0x[0-9a-f]+ module:overlay\((\d+)\)", line)
        if x:
            d[int(x.group(1), 16)] = int(x.group(2))
    return d


def elf_symbols_and_calls(data: bytes):
    shoff = struct.unpack_from("<I", data, 0x20)[0]
    shnum = struct.unpack_from("<H", data, 0x30)[0]
    secs = [struct.unpack_from("<10I", data, shoff + i * 40) for i in range(shnum)]
    syms = []
    for s in secs:
        if s[1] == 2:       # SHT_SYMTAB
            st = secs[s[6]]
            for i in range(s[5] // 16):
                nm, v, sz, info, _, sh = struct.unpack_from("<IIIBBH", data, s[4] + i * 16)
                name = data[st[4] + nm:data.index(b"\0", st[4] + nm)].decode()
                syms.append((name, v, sz, info, sh))
    calls = {}
    for s in secs:
        if s[1] in (4, 9):  # SHT_RELA, SHT_REL
            es = 12 if s[1] == 4 else 8
            for i in range(s[5] // es):
                off, info = struct.unpack_from("<II", data, s[4] + i * es)
                if info & 0xff == 10:   # R_ARM_THM_CALL
                    calls.setdefault(s[7], []).append((off, syms[info >> 8][0]))
    return syms, calls


def mismatches(objs):
    main_n2a, main_a2n = load_symbols(CFG)
    main_bin = (ROOT / "extract/usa/arm9/arm9.bin").read_bytes()
    mods, ovcalls, ovnames = {}, {}, {}

    def module(name):
        if name not in mods:
            n2a, a2n = load_symbols(CFG / "overlays" / name)
            base = next(b for r, b in OVERLAY_BASES.items() if int(name[2:]) in r)
            binary = (ROOT / f"extract/usa/arm9_overlays/{name}.bin").read_bytes()
            mods[name] = (n2a, {**main_a2n, **a2n}, binary, base)
        return mods[name]

    def calls_of(name):
        if name not in ovcalls:
            ovcalls[name] = overlay_calls(CFG if name == "main" else CFG / "overlays" / name)
        return ovcalls[name]

    def ovname(n, a):
        if n not in ovnames:
            ovnames[n] = load_symbols(CFG / "overlays" / f"ov{n:03d}")[1]
        return ovnames[n].get(a)

    out = []
    for obj in objs:
        m = re.search(r"/src/(ov\d\d\d)/", str(obj))
        if m:
            n2a, a2n, binary, base = module(m.group(1))
            rm = calls_of(m.group(1))
        else:
            n2a, a2n, binary, base = main_n2a, main_a2n, main_bin, 0x02000000
            rm = calls_of("main")
        syms, calls = elf_symbols_and_calls(Path(obj).read_bytes())
        for nm, v, sz, info, sh in syms:
            if info & 0xf != 2 or sz == 0 or nm not in n2a:
                continue
            ga = n2a[nm]
            for off, t in calls.get(sh, []):
                if not (v & ~1) <= off < (v & ~1) + sz:
                    continue
                o = off - (v & ~1)
                h1, h2 = struct.unpack_from("<HH", binary, ga + o - base)
                if h1 & 0xf800 != 0xf000:
                    continue
                x = ((h1 & 0x7ff) << 12) | ((h2 & 0x7ff) << 1)
                if x & 0x400000:
                    x -= 0x800000
                tg = ga + o + 4 + x
                if h2 & 0xf800 == 0xe800:   # blx: word-aligned target
                    tg &= ~3
                gn = a2n.get(tg, hex(tg))
                if (ga + o) in rm:
                    gn = ovname(rm[ga + o], tg) or gn
                if gn.startswith("0x") and tg >= 0x021558a0 and base == 0x02000000:
                    continue        # main calling into an overlay dsd didn't resolve
                if gn == t.replace("C2E", "C1E", 1):
                    continue        # base-object ctor called where the game names the complete one
                if gn != t:
                    out.append(f"{os.path.basename(obj)} {nm}+{o:#x}: ours {t}  game {gn}")
    return out


def all_objects():
    return sorted(str(p) for d in ("build/usa/src", "build/usa/libs") for p in (ROOT / d).rglob("*.o"))


def main():
    objs = sys.argv[1:] or all_objects()
    bad = mismatches(objs)
    for line in bad:
        print(line)
    print("mismatches", len(bad), file=sys.stderr)


if __name__ == "__main__":
    main()
