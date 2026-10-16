#!/usr/bin/env python3
"""Merges the auto-named symbols inside a data object into one named symbol.

Usage: python3 tools/merge_data.py NAME START END [--module main]
dsd names every address the code loads (data_0208c40c, data_0208c414, ...). When those are members of one table,
this renames the symbol at START to NAME, drops the other data_ symbols in [START, END) and points relocations
to them at NAME plus an addend, so code compiled against the table relocates the same way.
"""
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CFG = ROOT / "config/usa/arm9"


def main() -> None:
    name, start, end = sys.argv[1], int(sys.argv[2], 16), int(sys.argv[3], 16)
    module = sys.argv[sys.argv.index("--module") + 1] if "--module" in sys.argv else "main"
    cfg = CFG if module == "main" else CFG / "overlays" / re.sub(r"overlay\((\d+)\)", lambda m: f"ov{int(m.group(1)):03d}", module)
    syms = cfg / "symbols.txt"
    lines = syms.read_text().splitlines()
    out, dropped, found = [], set(), False
    for l in lines:
        m = re.match(r"^(\S+) (kind:\S+) addr:(0x[0-9a-f]+)", l)
        if m and start <= int(m.group(3), 16) < end:
            a = int(m.group(3), 16)
            if a == start:
                out.append(f"{name} {m.group(2)} addr:{m.group(3)}")
                found = True
                continue
            if not m.group(1).startswith("data_"):
                raise SystemExit(f"{m.group(1)} is inside the table and isn't auto-named")
            dropped.add(a)
            continue
        out.append(l)
    if not found:
        raise SystemExit(f"no symbol at {start:#010x}")
    syms.write_text("\n".join(out) + "\n")
    n = 0
    for path in CFG.rglob("relocs.txt"):
        text = path.read_text()

        def sub(m):
            nonlocal n
            to = int(m.group(1), 16)
            if to in dropped:
                n += 1
                return f"to:{start:#010x} add:{to - start:#x} module:{module}"
            return m.group(0)
        new = re.sub(rf"to:(0x[0-9a-f]+) module:{re.escape(module)}(?= |$)", sub, text, flags=re.M)
        if new != text:
            path.write_text(new)
    print(f"{name}: dropped {len(dropped)} symbols, rewrote {n} relocations")


if __name__ == "__main__":
    main()
