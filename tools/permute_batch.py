#!/usr/bin/env python3
"""Runs tools/permute.py over the closest non-matching functions in the report, one after another.

Usage: python3 tools/permute_batch.py [--min PCT] [--max-size BYTES] [-t SECONDS] [-j JOBS] [--units PREFIX ...]
       [--list]

Candidates come from build/report.json (functions below 100% but at least --min), restricted to units under the
given prefixes (default: every src/ unit). The definition is found by demangling the symbol (c++filt) and looking
for "Class::name(" (or "name(" for extern "C" functions) in the unit's source; template instances and functions not
defined in the unit's own file are skipped. Results go to build/permute/<symbol>/, and a summary line per function to
build/permute/batch.log. Matches are only written there: nothing in src/ is changed.
"""
import argparse
import json
import re
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent


def definition_start(src: Path, symbol: str):
    if not symbol.startswith("_Z"):
        name = symbol
    else:
        dem = subprocess.run(["c++filt", symbol], capture_output=True, text=True).stdout.strip()
        if "<" in dem or dem == symbol:
            return None
        name = dem.split("(")[0]
    text = src.read_text()
    found = None
    for m in re.finditer(r"(?<![\w:])" + re.escape(name) + r"\(", text):
        line_start = text.rfind("\n", 0, m.start()) + 1
        if text[line_start:m.start()].strip().startswith("//"):
            continue
        j, semi = text.find("{", m.start()), text.find(";", m.start())
        if j != -1 and (semi == -1 or j < semi) and text.count("\n", m.start(), j) < 4:
            if found:
                return None     # overloaded: the definition can't be told apart by its name
            found = name + "("
    return found


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--min", type=float, default=90)
    ap.add_argument("--max-size", type=int, default=4096)
    ap.add_argument("-t", type=float, default=600)
    ap.add_argument("-j", type=int, default=9)
    ap.add_argument("--units", nargs="*", default=["src/"])
    ap.add_argument("--list", action="store_true")
    ap.add_argument("--improved", help="only the functions an earlier batch log shows improving (resumed from their best)")
    a = ap.parse_args()
    report = json.load(open(ROOT / "build/report.json"))
    cands = []
    for u in report["units"]:
        if not any(u["name"].startswith(p) for p in a.units):
            continue
        src = ROOT / (u["name"] + ".cpp")
        if not src.exists():
            src = ROOT / (u["name"] + ".c")
            if not src.exists():
                continue
        for f in u.get("functions", []):
            pct = f.get("fuzzy_match_percent") or 0
            size = int(f.get("size", 0))
            dup = re.sub(r"([CD])2E", r"\g<1>1E", f["name"])    # (base-object ctors/dtors repeat the complete ones)
            if dup != f["name"] and any(g["name"] == dup for g in u["functions"]):
                continue
            if a.min <= pct < 100 and size <= a.max_size and not f["name"].startswith("__sinit"):
                start = definition_start(src, f["name"])
                if start:
                    cands.append((pct, size, src, f["name"], start))
    if a.improved:
        better = set()
        for l in open(a.improved):
            m = re.search(r" (\S+): base score (\d+); done: best (\d+)", l)
            if m and 0 < int(m.group(3)) < int(m.group(2)):
                better.add(m.group(1))
        cands = [c for c in cands if c[3] in better]
    cands.sort(key=lambda c: (-c[0], c[1]))
    if a.list:
        for pct, size, src, sym, start in cands:
            print(f"{pct:6.2f} {size:5d} {src.relative_to(ROOT)} {sym} {start!r}")
        print(len(cands), "candidates")
        return
    log = ROOT / "build/permute/batch.log"
    log.parent.mkdir(parents=True, exist_ok=True)
    for pct, size, src, sym, start in cands:
        t0 = time.time()
        r = subprocess.run([sys.executable, str(ROOT / "tools/permute.py"), str(src.relative_to(ROOT)), sym, start,
                            "-j", str(a.j), "-t", str(a.t)] + (["--resume"] if a.improved else []), cwd=ROOT, capture_output=True, text=True)
        lines = (r.stdout + r.stderr).strip().split("\n")
        base = next((l for l in lines if l.startswith("base score")), "?")
        last = lines[-1] if lines else "?"
        matched = (ROOT / "build/permute" / sym / "best-0.cpp").exists()
        msg = (f"{'MATCH' if matched else '     '} {pct:6.2f} {src.relative_to(ROOT)} {sym}: {base}; {last} "
               f"({time.time() - t0:.0f}s)")
        print(msg, flush=True)
        with open(log, "a") as fp:
            fp.write(msg + "\n")


if __name__ == "__main__":
    main()
