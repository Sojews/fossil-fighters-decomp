#!/usr/bin/env python3
"""Compiles a C file and compares each function in it with the game function of the same name.

Usage: python3 tools/try_match.py src/foo.c [function ...]
Run `python3 tools/configure.py` first (the compile command is taken from build.ninja).
Install capstone (`pip install capstone`) to get a side-by-side disassembly of mismatches.
"""
import re
import shlex
import struct
import subprocess
import sys
import tempfile
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CFG = ROOT / "config/usa/arm9"
EX = ROOT / "extract/usa"


def modules():
    mods = {"main": (CFG, EX / "arm9/arm9.bin", 0x02000000)}
    for name, kind in (("itcm", "itcm"), ("dtcm", "dtcm")):
        base = int(re.search(r"base_address: (\d+)", (EX / f"arm9/{kind}.yaml").read_text()).group(1))
        mods[name] = (CFG / name, EX / f"arm9/{kind}.bin", base)
    for i, base in re.findall(r"- id: (\d+)\n\s+base_address: (\d+)", (EX / "arm9_overlays/overlays.yaml").read_text()):
        n = f"ov{int(i):03d}"
        mods[n] = (CFG / f"overlays/{n}", EX / f"arm9_overlays/{n}.bin", int(base))
    return mods


def game_symbols():
    syms = {}
    for mod, (cdir, binp, base) in modules().items():
        for line in (cdir / "symbols.txt").read_text().splitlines():
            m = re.match(r"^(\S+) kind:function\((\w+),size=(0x[0-9a-f]+)\) addr:(0x[0-9a-f]+)", line)
            if m:
                syms[m.group(1)] = (mod, binp, base, int(m.group(4), 16), int(m.group(3), 16), m.group(2) == "thumb")
    return syms


def compile_c(src: Path, out_dir: Path) -> Path:
    ninja = (ROOT / "build.ninja").read_text()
    cmd = re.search(r"rule cc\n  command = (.*?) -MD -c", ninja).group(1)
    edge = re.search(rf"build \S+: cc(?:_partial)? {re.escape(str(src.relative_to(ROOT)))}(?: \|[^\n]*)?\n(?:  .*\n)*?  extra_cflags = (.*)\n", ninja)
    cmd = cmd.replace("$extra_cflags", edge.group(1) if edge else "")
    r = subprocess.run(shlex.split(cmd) + ["-c", str(src), "-o", str(out_dir) + "/"], cwd=ROOT,
                       capture_output=True, text=True)
    obj = out_dir / (src.stem + ".o")
    if not obj.exists():            # wibo exits 0 even when mwcc fails
        sys.exit(r.stdout + r.stderr)
    if r.stdout.strip():
        print(r.stdout.strip())
    return obj


def read_elf(path: Path):
    b = path.read_bytes()
    shoff, = struct.unpack_from("<I", b, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from("<HHH", b, 0x2E)
    secs = [struct.unpack_from("<IIIIIIIIII", b, shoff + i * shentsize) for i in range(shnum)]
    data = lambda s: b[s[4]:s[4] + s[5]]
    cstr = lambda tab, off: tab[off:tab.index(b"\0", off)].decode()
    names = [cstr(data(secs[shstrndx]), s[0]) for s in secs]
    syms, rels = [], defaultdict(dict)
    for s in secs:
        if s[1] == 2:
            strtab, d = data(secs[s[6]]), data(s)
            for j in range(len(d) // 16):
                nm, val, size, info, _, shndx = struct.unpack_from("<IIIBBH", d, j * 16)
                syms.append((cstr(strtab, nm), val, size, info & 0xF, shndx))
    for s in secs:
        if s[1] in (4, 9):   # RELA or REL
            d, ent = data(s), 12 if s[1] == 4 else 8
            for j in range(len(d) // ent):
                off, info = struct.unpack_from("<II", d, j * ent)
                rels[s[7]][off] = syms[info >> 8][0]
    return secs, names, syms, rels, data


def disasm(code: bytes, addr: int, thumb: bool):
    try:
        import capstone
    except ImportError:
        return None
    md = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB if thumb else capstone.CS_MODE_ARM)
    out, off = [], 0
    while off < len(code):
        ins = next(md.disasm(code[off:], addr + off, 1), None)
        if ins is None:
            out.append((off, code[off:off + 2].hex(), ".data"))
            off += 2
            continue
        out.append((off, ins.bytes.hex(), f"{ins.mnemonic} {ins.op_str}"))
        off += ins.size
    return out


def main() -> None:
    src = Path(sys.argv[1]).resolve()
    only = set(sys.argv[2:])
    syms = game_symbols()
    with tempfile.TemporaryDirectory() as tmp:
        secs, names, osyms, rels, data = read_elf(compile_c(src, Path(tmp)))
        ok = total = 0
        for name, val, size, typ, shndx in osyms:
            if typ != 2 or not size or (only and name not in only):
                continue
            total += 1
            if name not in syms:
                print(f"?? {name}: no game function with this name in symbols.txt")
                continue
            mod, binp, base, addr, gsize, thumb = syms[name]
            ours = data(secs[shndx])[val:val + size]
            theirs = binp.read_bytes()[addr - base:addr - base + gsize]
            mask = bytearray(b"\xff" * size)
            for off in rels[shndx]:
                if val <= off < val + size:
                    mask[off - val:off - val + 4] = b"\0" * min(4, size - (off - val))
            same = size == gsize and all((a ^ b) & m == 0 for a, b, m in zip(ours, theirs, mask))
            if same:
                ok += 1
                print(f"OK {name} ({size:#x} bytes)")
                continue
            print(f"XX {name}: ours {size:#x} bytes, game {gsize:#x} bytes")
            a, b = disasm(ours, addr, thumb), disasm(theirs, addr, thumb)
            if a is None:
                continue
            for i in range(max(len(a), len(b))):
                l = a[i] if i < len(a) else (0, "", "")
                r = b[i] if i < len(b) else (0, "", "")
                masked = bool(l[1]) and any(mask[k] == 0 for k in range(l[0], min(l[0] + len(l[1]) // 2, size)))
                same_op = l[2].split(" ")[0] == r[2].split(" ")[0]
                mark = " " if l[2] == r[2] or (masked and same_op) else "|"
                print(f"  {l[2]:<34.34} {mark} {r[2]}")
        print(f"{ok}/{total} functions match")


if __name__ == "__main__":
    main()
