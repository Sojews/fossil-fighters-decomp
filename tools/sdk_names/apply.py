#!/usr/bin/env python3
"""Apply match.py output to the decomp's symbols.txt files.  Usage: python3 tools/sdk_names/apply.py build/sdk_names.tsv

Only placeholder names (func_*/data_*) are replaced. A name that would be used twice keeps it on the
first address and gets an address suffix elsewhere (e.g. static helpers present in two files)."""
import re, sys
from collections import defaultdict
from pathlib import Path

CFG = Path(__file__).resolve().parents[2] / "config/usa/arm9"
paths = {"main": CFG / "symbols.txt", "itcm": CFG / "itcm/symbols.txt", "dtcm": CFG / "dtcm/symbols.txt"}
paths.update({p.parent.name: p for p in (CFG / "overlays").glob("*/symbols.txt")})

rows = [l.split("\t") for l in Path(sys.argv[1]).read_text().splitlines() if l and not l.startswith("#")]
existing = set()
for p in paths.values():
    existing |= {l.split(" ", 1)[0] for l in p.read_text().splitlines() if l}

by_name = defaultdict(list)
for mod, addr, cur, new, how, obj, *_ in rows:
    if re.match(r"(func|data)_", cur):
        by_name[new].append((mod, int(addr, 16), cur, how))

rename = defaultdict(dict)            # module -> {old: new}
for new, uses in by_name.items():
    uses.sort(key=lambda u: (u[3] != "sig", u[1]))
    # Several reloc-only hits of a data symbol at nearby addresses are offsets into the same object.
    if all(u[3] == "reloc" for u in uses) and len({u[0] for u in uses}) == 1:
        uses = uses[:1]
    for i, (mod, addr, cur, how) in enumerate(uses):
        name = new if i == 0 and new not in existing else f"{new}_{addr:08x}"
        rename[mod][cur] = name
        existing.add(name)

n = 0
for mod, mapping in rename.items():
    p = paths[mod]
    out = []
    for line in p.read_text().splitlines():
        old = line.split(" ", 1)[0]
        if old in mapping:
            line = mapping[old] + line[len(old):]
            n += 1
        out.append(line)
    p.write_text("\n".join(out) + "\n")
print(f"renamed {n} symbols")
