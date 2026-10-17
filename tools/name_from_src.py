#!/usr/bin/env python3
"""Names game functions after the functions in a source file that compile to the same bytes.

Usage: python3 tools/name_from_src.py src/foo.cpp [--dry-run] [--anywhere]
Compiles the file and, for each function in it whose name isn't in symbols.txt yet, looks for auto-named
(func_XXXXXXXX) game functions of the same size whose bytes match, ignoring the bytes that relocations fill in.
Only the .text range the file has in delinks.txt is searched, when it has one. A function is renamed only when
exactly one game function matches it, so functions that don't match yet still need naming by hand.
"""
import re
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import try_match as t  # noqa: E402


def text_range(src: Path):
    rel = str(src.relative_to(t.ROOT))
    for cdir in [t.CFG] + sorted(t.CFG.glob("*/")) + sorted((t.CFG / "overlays").glob("*/")):
        p = cdir / "delinks.txt"
        if not p.exists():
            continue
        m = re.search(rf"^{re.escape(rel)}:\n(?:    .*\n)*?    \.text\s+start:(0x[0-9a-f]+) end:(0x[0-9a-f]+)",
                      p.read_text(), re.M)
        if m:
            return int(m.group(1), 16), int(m.group(2), 16)
    return None


def main() -> None:
    src = Path(sys.argv[1]).resolve()
    dry = "--dry-run" in sys.argv
    anywhere = "--anywhere" in sys.argv     # also search outside the file's own range (template instances)
    syms = t.game_symbols()
    rng = text_range(src)
    cache = {}
    renames = {}
    with tempfile.TemporaryDirectory() as tmp:
        secs, names, osyms, rels, data = t.read_elf(t.compile_c(src, Path(tmp)))
        for name, val, size, typ, shndx in osyms:
            if typ != 2 or not size or name in syms:
                continue
            ours = data(secs[shndx])[val:val + size]
            mask = bytearray(b"\xff" * size)
            for off in rels[shndx]:
                if val <= off < val + size:
                    mask[off - val:off - val + 4] = b"\0" * min(4, size - (off - val))
            hits = []
            for gname, entries in syms.items():
                if not gname.startswith("func_"):
                    continue
                for mod, binp, base, addr, gsize, thumb in entries:
                    if gsize != size:
                        continue
                    if rng and not anywhere and not rng[0] <= addr < rng[1]:
                        continue
                    b = cache.setdefault(binp, binp.read_bytes())
                    theirs = b[addr - base:addr - base + gsize]
                    if all((x ^ y) & m == 0 for x, y, m in zip(ours, theirs, mask)):
                        hits.append(gname)
            if len(hits) == 1:
                renames.setdefault(hits[0], []).append(name)
            elif hits:
                print(f"?? {name}: {len(hits)} candidates ({', '.join(hits[:4])})")
            else:
                print(f"-- {name}: no match")
    for gname, names in list(renames.items()):
        if len(names) == 1:
            renames[gname] = names[0]
            print(f"{gname} -> {names[0]}")
        else:   # e.g. a C1/C2 or D1/D2 pair, or empty functions that compile the same
            print(f"?? {gname} matches {', '.join(names)}: name it by hand")
            del renames[gname]
    if dry or not renames:
        return
    for p in t.CFG.rglob("symbols.txt"):
        lines = p.read_text().splitlines()
        changed = False
        for i, l in enumerate(lines):
            tok = l.split(" ", 1)[0]
            if tok in renames:
                lines[i] = renames[tok] + l[len(tok):]
                changed = True
        if changed:
            p.write_text("\n".join(lines) + "\n")
    print(f"renamed {len(renames)}")


if __name__ == "__main__":
    main()
