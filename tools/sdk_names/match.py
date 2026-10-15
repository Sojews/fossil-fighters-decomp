#!/usr/bin/env python3
"""Match functions from compiled reference objects against the game's code by masked bytes.

Usage: python3 tools/sdk_names/match.py tools/sdk_names/obj/* > build/sdk_names.tsv
Prints: module  address  current_name  new_name  how(sig|reloc)  source_object
"""
import re, struct, sys
from collections import defaultdict
from pathlib import Path

D = Path(__file__).resolve().parents[2]
CFG = D / "config/usa/arm9"
EX = D / "extract/usa"

# ---------- game side ----------
MODULES = {"main": (CFG, EX / "arm9/arm9.bin", 0x02000000),
           "itcm": (CFG / "itcm", EX / "arm9/itcm.bin", 0x01FF8000),
           "dtcm": (CFG / "dtcm", EX / "arm9/dtcm.bin", 0x027E0000)}
ovl = re.findall(r"- id: (\d+)\n\s+base_address: (\d+)", (EX / "arm9_overlays/overlays.yaml").read_text())
for i, base in ovl:
    MODULES[f"ov{int(i):03d}"] = (CFG / f"overlays/ov{int(i):03d}", EX / f"arm9_overlays/ov{int(i):03d}.bin", int(base))

SYM_RE = re.compile(r"^(\S+) kind:(function|data|bss|label)\(([^)]*)\) addr:(0x[0-9a-f]+)")
game_funcs = []                       # (module, addr, size, name, thumb)
game_syms = {}                        # (module, addr) -> name
by_size = defaultdict(list)
mem = {}
for mod, (cdir, binp, base) in MODULES.items():
    mem[mod] = (binp.read_bytes(), base)
    for line in (cdir / "symbols.txt").read_text().splitlines():
        m = SYM_RE.match(line)
        if not m:
            continue
        name, kind, attrs, addr = m.group(1), m.group(2), m.group(3), int(m.group(4), 16)
        game_syms[(mod, addr)] = name
        if kind == "function":
            sm = re.search(r"size=(0x[0-9a-f]+)", attrs)
            if sm:
                f = (mod, addr, int(sm.group(1), 16), name, attrs.startswith("thumb"))
                game_funcs.append(f)
                by_size[f[2]].append(f)

game_relocs = {}                      # (module, from) -> (to, to_module)
REL_RE = re.compile(r"from:(0x[0-9a-f]+) kind:(\S+) to:(0x[0-9a-f]+)(?: add:\S+)? module:(\S+)")
for mod, (cdir, _, _) in MODULES.items():
    for line in (cdir / "relocs.txt").read_text().splitlines():
        m = REL_RE.match(line)
        if m:
            game_relocs[(mod, int(m.group(1), 16))] = (int(m.group(3), 16), m.group(4))

def game_bytes(mod, addr, size):
    data, base = mem[mod]
    return data[addr - base: addr - base + size]

# ---------- reference side (minimal ELF32 reader) ----------
def read_elf(path):
    b = path.read_bytes()
    shoff, = struct.unpack_from("<I", b, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from("<HHH", b, 0x2E)
    secs = [struct.unpack_from("<IIIIIIIIII", b, shoff + i * shentsize) for i in range(shnum)]
    def sdata(s): return b[s[4]:s[4] + s[5]]
    shstr = sdata(secs[shstrndx])
    def cstr(tab, off): return tab[off:tab.index(b"\0", off)].decode(errors="replace")
    names = [cstr(shstr, s[0]) for s in secs]
    syms, rels = [], defaultdict(dict)
    for i, s in enumerate(secs):
        if s[1] == 2:  # SYMTAB
            strtab = sdata(secs[s[6]])
            d = sdata(s)
            for j in range(len(d) // 16):
                nm, val, size, info, other, shndx = struct.unpack_from("<IIIBBH", d, j * 16)
                syms.append((cstr(strtab, nm), val, size, info & 0xF, shndx))
    for i, s in enumerate(secs):
        if s[1] in (9, 4):  # REL / RELA
            target = s[7]
            d = sdata(s)
            ent = 8 if s[1] == 9 else 12
            for j in range(len(d) // ent):
                off, info = struct.unpack_from("<II", d, j * ent)
                rels[target][off] = (info & 0xFF, info >> 8)
    return secs, names, syms, rels, sdata

ref_funcs = []                        # (name, bytes, mask, relocs{off:name}, obj)
for objdir in sys.argv[1:]:
    for obj in sorted(Path(objdir).rglob("*.o")):
        secs, names, syms, rels, sdata = read_elf(obj)
        for name, val, size, typ, shndx in syms:
            if typ != 2 or size < 4 or shndx == 0 or shndx >= len(secs):
                continue
            code = sdata(secs[shndx])[val:val + size]
            mask = bytearray(b"\xff" * size)
            rnames = {}
            for off, (rtype, symi) in rels[shndx].items():
                if val <= off < val + size:
                    for k in range(4):
                        if off - val + k < size:
                            mask[off - val + k] = 0
                    rnames[off - val] = syms[symi][0] if symi < len(syms) else "?"
            ref_funcs.append((name, bytes(code), bytes(mask), rnames, str(obj.relative_to(Path(objdir).parent))))

# ---------- match ----------
def masked(data, mask): return bytes(a & m for a, m in zip(data, mask))

hits = defaultdict(set)               # game func -> {ref names}
ref_hits = defaultdict(set)           # ref name -> {game funcs}
ref_by_name = {}
for name, code, mask, rnames, obj in ref_funcs:
    want = masked(code, mask)
    for f in by_size.get(len(code), []):
        if masked(game_bytes(f[0], f[1], f[2]), mask) == want:
            hits[f].add(name)
            ref_hits[name].add(f)
            ref_by_name[name] = (code, mask, rnames, obj)

named = {}                            # (module, addr) -> (new name, how, obj)
for f, refs in hits.items():
    if len(refs) != 1:
        continue
    (r,) = refs
    # Identical-looking tiny functions match everywhere; only trust unique pairs.
    if len(ref_hits[r]) != 1 and f[2] < 0x20:
        continue
    named[(f[0], f[1])] = (r, "sig", ref_by_name[r][3])

# Small functions need a neighbour from the same reference object nearby, or they're likely coincidences.
def supported(key, obj, size):
    if size >= 0x40:
        return True
    return any(k != key and k[0] == key[0] and abs(k[1] - key[1]) < 0x1000 and v[2] == obj and sizes[k] >= 0x20
               for k, v in named.items())
sizes = {(f[0], f[1]): f[2] for f in game_funcs}
named = {k: v for k, v in named.items() if supported(k, v[2], sizes[k])}

# Propagate through relocations of matched functions.
changed = True
while changed:
    changed = False
    for (mod, addr), (r, how, obj) in list(named.items()):
        if how != "sig":
            continue
        code, mask, rnames, _ = ref_by_name[r]
        for off, tname in rnames.items():
            to = game_relocs.get((mod, addr + off))
            if not to or tname.startswith(("@", ".", "$")) or tname == "?":
                continue
            key = (to[1], to[0] & ~1)
            if key in game_syms and key not in named:
                named[key] = (tname, "reloc", obj)
                changed = True

# Report only real renames; flag name collisions.
counts = defaultdict(int)
for v in named.values():
    counts[v[0]] += 1
for (mod, addr), (r, how, obj) in sorted(named.items()):
    cur = game_syms.get((mod, addr), "?")
    flag = "DUP" if counts[r] > 1 else ""
    print(f"{mod}\t{addr:#010x}\t{cur}\t{r}\t{how}\t{obj}\t{flag}")
print(f"# ref functions: {len(ref_funcs)}, game functions: {len(game_funcs)}, "
      f"sig matches: {sum(1 for v in named.values() if v[1]=='sig')}, "
      f"via relocs: {sum(1 for v in named.values() if v[1]=='reloc')}, "
      f"ambiguous: {sum(1 for f, r in hits.items() if len(r) > 1)}", file=sys.stderr)
