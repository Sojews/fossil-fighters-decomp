#!/usr/bin/env python3
"""Unattended permuter sweep: permutes the closest near-misses one by one, gates every candidate the permuter writes
(full build + report + call-target check against a baseline), and commits each clean win on a local branch.

Usage: .venv/bin/python tools/overnight_sweep.py [--branch NAME | --here] [--min 95] [--max 100] [--max-size 1500]
           [--units src/ ...] [--exclude PREFIX ...] [--only SYMBOL ...] [-t 600] [-j JOBS] [--hours 8] [--limit N]
           [--tries 3] [--mine] [--resume] [--no-reduce] [--again] [--dry-run]

Run it from a dedicated worktree whose src/, include/, libs/ and config/ are clean (it refuses otherwise), with a
built tree (`python3 tools/configure.py usa && ninja`). One command, no LLM:

  1. Creates and switches to the branch NAME (default sweep-<date>-<time>) from HEAD, or commits on the current HEAD
     with --here.
  2. Builds and writes the baseline: build/sweep/base.json (objdiff report) and build/sweep/calls_base.txt
     (tools/callcheck.py output).
  3. Picks candidates from the baseline exactly like tools/permute_batch.py (functions in [--min, --max) under --units,
     at most --max-size bytes, defined by name in their unit's own file; templates/overloads/sinits are skipped),
     closest first. --exclude drops unit prefixes (e.g. a peer's overlays); --only keeps the given symbols.
  4. Per function: runs tools/permute.py for -t seconds (skipped with --mine, which only re-gates the
     build/permute/<symbol>/best-*.cpp files earlier runs left), then gates its --tries lowest-score outputs. The
     gate passes when the build succeeds, no function that was 100% in the baseline drops, no other function loses
     more than --fuzz percent, tools/callcheck.py reports no new call-target mismatch, and the function itself gains
     at least --min-gain percent in build/report.json (the permuter's own score doesn't track objdiff %).
  5. The best passing candidate is reduced (each changed hunk reverted in turn, kept reverted while the function's
     % and the gate hold; --no-reduce skips this), committed as "<function> A->B: permuter candidate (score N)", and
     becomes the new baseline. The source file is restored after every failed try, also on Ctrl-C.

Progress goes to stdout and build/sweep/sweep.log; build/sweep/state.json remembers which (symbol, file contents)
pairs were tried, so a rerun skips them unless --again. Permuter edits can change semantics (dropped branches,
reordered side effects, reads of uninitialised locals): review every commit on the branch before keeping it.
"""
import argparse
import difflib
import hashlib
import json
import signal
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import callcheck  # noqa: E402
import permute_batch  # noqa: E402

OUT = ROOT / "build/sweep"
LOG = OUT / "sweep.log"
OBJDIFF = ROOT / "tools/bin/objdiff-cli"


def log(msg):
    line = f"[{time.strftime('%H:%M:%S')}] {msg}"
    print(line, flush=True)
    with open(LOG, "a") as fp:
        fp.write(line + "\n")


def git(*args, check=True):
    r = subprocess.run(["git", *args], cwd=ROOT, capture_output=True, text=True)
    if check and r.returncode:
        sys.exit(f"git {' '.join(args)} failed: {r.stderr.strip()}")
    return r.stdout.strip()


# ---- building and gating

def build_and_report(path: Path):
    """Builds everything and writes the objdiff report to path. Returns (report, call mismatches) or None."""
    r = subprocess.run(["ninja"], cwd=ROOT, capture_output=True, text=True)
    (OUT / "ninja.log").write_text(r.stdout + r.stderr)
    if r.returncode:
        return None
    subprocess.run(["ninja", "objdiff.json"], cwd=ROOT, capture_output=True)
    r = subprocess.run([str(OBJDIFF), "report", "generate", "-o", str(path)], cwd=ROOT, capture_output=True)
    if r.returncode or not path.exists():
        return None
    return json.loads(path.read_text()), set(callcheck.mismatches(callcheck.all_objects()))


def scores(report):
    return {f["name"]: f.get("fuzzy_match_percent") or 0 for u in report["units"] for f in u.get("functions", [])}


def measures(report):
    m = report["measures"]
    return f"{m.get('matched_functions')}/{m.get('total_functions')} {float(m.get('matched_code_percent', 0)):.4f}%"


class Gate:
    def __init__(self, fuzz):
        self.fuzz = fuzz
        res = build_and_report(OUT / "base.json")
        if res is None:
            sys.exit("the baseline doesn't build: see build/sweep/ninja.log")
        self.report, self.calls = res
        self.scores = scores(self.report)
        (OUT / "calls_base.txt").write_text("\n".join(sorted(self.calls)) + "\n")

    def check(self, symbol):
        """(passed, the symbol's new %, why, result) for the tree as it is now."""
        res = build_and_report(OUT / "try.json")
        if res is None:
            return False, None, "build failed", None
        report, calls = res
        now = scores(report)
        pct = now.get(symbol)
        lost = [k for k, v in self.scores.items() if v == 100 and now.get(k, 0) != 100]
        if lost:
            return False, pct, f"{len(lost)} lost matches (e.g. {lost[0]})", res
        worse = [k for k, v in self.scores.items() if k != symbol and now.get(k, 0) < v - self.fuzz]
        if worse:
            return False, pct, f"{len(worse)} functions dropped (e.g. {worse[0]})", res
        new_calls = calls - self.calls
        if new_calls:
            return False, pct, f"new call-target mismatch: {sorted(new_calls)[0]}", res
        return True, pct, "ok", res

    def accept(self, res):
        self.report, self.calls = res
        self.scores = scores(self.report)
        (OUT / "base.json").write_text(json.dumps(self.report))
        (OUT / "calls_base.txt").write_text("\n".join(sorted(self.calls)) + "\n")


# ---- candidates

def demangled(symbol):
    if not symbol.startswith("_Z"):
        return symbol
    dem = subprocess.run(["c++filt", symbol], capture_output=True, text=True).stdout.strip()
    return dem.split("(")[0] or symbol


def permuter_outputs(symbol, tries):
    d = ROOT / "build/permute" / symbol
    bests = sorted(d.glob("best-*.cpp"), key=lambda p: int(p.stem[5:])) if d.exists() else []
    return bests[:tries]


def hunks(old, new):
    sm = difflib.SequenceMatcher(None, old, new, autojunk=False)
    return [op for op in sm.get_opcodes() if op[0] != "equal"]


def apply_reverted(old, new, ops, reverted):
    """new with the hunks whose index is in reverted put back to old."""
    out, j = [], 0
    for idx, (tag, i1, i2, j1, j2) in enumerate(ops):
        out += new[j:j1]
        out += old[i1:i2] if idx in reverted else new[j1:j2]
        j = j2
    return out + new[j:]


def reduce(src, old_text, new_text, symbol, target, gate, best_res):
    """Reverts each changed hunk that the gain doesn't need. Returns (text, result)."""
    old, new = old_text.splitlines(keepends=True), new_text.splitlines(keepends=True)
    ops = hunks(old, new)
    if len(ops) <= 1 or len(ops) > 40:
        return new_text, best_res
    reverted = set()
    for idx in reversed(range(len(ops))):
        trial = "".join(apply_reverted(old, new, ops, reverted | {idx}))
        src.write_text(trial)
        ok, pct, _, res = gate.check(symbol)
        if ok and pct is not None and pct >= target - 1e-9:
            reverted.add(idx)
            best_res = res
    text = "".join(apply_reverted(old, new, ops, reverted))
    src.write_text(text)
    if reverted:
        log(f"  reduced: {len(reverted)} of {len(ops)} hunks reverted")
    return text, best_res


# ---- main loop

def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--branch", help="branch to create for the wins (default sweep-<date>-<time>)")
    ap.add_argument("--here", action="store_true", help="commit on the current HEAD instead of a new branch")
    ap.add_argument("--min", type=float, default=95)
    ap.add_argument("--max", type=float, default=100)
    ap.add_argument("--max-size", type=int, default=1500)
    ap.add_argument("--units", nargs="*", default=["src/"])
    ap.add_argument("--exclude", nargs="*", default=[], help="unit prefixes to leave alone")
    ap.add_argument("--only", nargs="*", default=[], help="only these (mangled) symbols")
    ap.add_argument("-t", type=float, default=600, help="permuter seconds per function")
    ap.add_argument("-j", type=int, default=6, help="permuter jobs")
    ap.add_argument("--hours", type=float, default=8, help="stop starting new functions after this long")
    ap.add_argument("--limit", type=int, default=0, help="at most this many functions (0 = all)")
    ap.add_argument("--tries", type=int, default=3, help="gate this many of the lowest-score outputs")
    ap.add_argument("--min-gain", type=float, default=0.005, help="percent the function must gain")
    ap.add_argument("--fuzz", type=float, default=0.001, help="percent another function may lose")
    ap.add_argument("--mine", action="store_true", help="don't run the permuter; gate existing outputs only")
    ap.add_argument("--resume", action="store_true", help="permuter starts from its best earlier output")
    ap.add_argument("--no-reduce", action="store_true")
    ap.add_argument("--again", action="store_true", help="also retry (symbol, file) pairs tried before")
    ap.add_argument("--dry-run", action="store_true", help="list the candidates and stop")
    a = ap.parse_args()

    OUT.mkdir(parents=True, exist_ok=True)
    base_report = json.loads((ROOT / "build/report.json").read_text()) if (ROOT / "build/report.json").exists() else None
    if a.dry_run:
        if base_report is None:
            sys.exit("build/report.json is missing: build and generate the report first")
        cands = [c for c in permute_batch.candidates(base_report, a.units, a.min, a.max, a.max_size)
                 if not any(str(c[2].relative_to(ROOT)).startswith(p) for p in a.exclude)
                 and (not a.only or c[3] in a.only)]
        cands.sort(key=lambda c: (-c[0], c[1]))
        for pct, size, src, sym, start in cands[:a.limit or None]:
            print(f"{pct:6.2f} {size:5d} {src.relative_to(ROOT)} {sym}")
        print(len(cands), "candidates")
        return

    dirty = git("status", "--porcelain", "--", "src", "include", "libs", "config")
    if dirty:
        sys.exit("src/include/libs/config have uncommitted changes; commit or clean them first:\n" + dirty)
    if not a.here:
        branch = a.branch or time.strftime("sweep-%Y%m%d-%H%M")
        git("switch", "-c", branch)
        log(f"on new branch {branch} at {git('rev-parse', '--short', 'HEAD')}")
    else:
        log(f"committing on the current HEAD {git('rev-parse', '--short', 'HEAD')}")

    gate = Gate(a.fuzz)
    log(f"baseline {measures(gate.report)}, {len(gate.calls)} call-target mismatches")
    cands = [c for c in permute_batch.candidates(gate.report, a.units, a.min, a.max, a.max_size)
             if not any(str(c[2].relative_to(ROOT)).startswith(p) for p in a.exclude)
             and (not a.only or c[3] in a.only)]
    cands.sort(key=lambda c: (-c[0], c[1]))
    if a.limit:
        cands = cands[:a.limit]
    log(f"{len(cands)} candidates")

    state_path = OUT / "state.json"
    state = json.loads(state_path.read_text()) if state_path.exists() else {}
    deadline = time.time() + a.hours * 3600
    current = {}        # the file being edited, for the signal handler

    def restore(*_):
        if current:
            current["src"].write_text(current["text"])
        sys.exit("interrupted; source restored")
    signal.signal(signal.SIGINT, restore)
    signal.signal(signal.SIGTERM, restore)

    wins = 0
    for pct0, size, src, sym, start in cands:
        if time.time() > deadline:
            log("time budget used up")
            break
        rel = str(src.relative_to(ROOT))
        text = src.read_text()
        key = hashlib.sha1(text.encode()).hexdigest()
        if not a.again and state.get(sym) == key:
            continue
        before = gate.scores.get(sym, pct0)
        if before >= 100:
            continue
        name = demangled(sym)
        log(f"{name} ({rel}, {size} B) at {before:.2f}%")
        if not a.mine:
            cmd = [sys.executable, str(ROOT / "tools/permute.py"), rel, sym, start, "-j", str(a.j), "-t", str(a.t)]
            if a.resume:
                cmd.append("--resume")
            try:
                r = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True, timeout=a.t + 300)
                lines = (r.stdout + r.stderr).strip().splitlines()
                summary = "; ".join(l for l in lines if l.startswith(("base score", "done")))
                log("  permuter: " + (summary or (lines[-1] if lines else "no output")))
            except subprocess.TimeoutExpired:
                log("  permuter timed out")
        outputs = permuter_outputs(sym, a.tries)
        best = None     # (pct, candidate text, path, result)
        current.update(src=src, text=text)
        try:
            for path in outputs:
                cand = path.read_text()
                if cand == text:
                    continue
                src.write_text(cand)
                ok, pct, why, res = gate.check(sym)
                log(f"  {path.name}: {'%.2f' % pct if pct is not None else '?'}% {why}")
                if ok and pct is not None and pct >= before + a.min_gain and (best is None or pct > best[0]):
                    best = (pct, cand, path, res)
            if best is None:
                src.write_text(text)
            else:
                pct, cand, path, res = best
                src.write_text(cand)
                if not a.no_reduce:
                    cand, res = reduce(src, text, cand, sym, pct, gate, res)
                ok, pct2, why, res = gate.check(sym)    # (the tree as committed)
                if not ok or pct2 is None or pct2 < before + a.min_gain:
                    log(f"  final check failed ({why}); restored")
                    src.write_text(text)
                else:
                    gate.accept(res)
                    git("add", rel)
                    verb = "matched" if pct2 >= 100 else f"{before:.2f}->{pct2:.2f}"
                    msg = (f"{name} {verb}: permuter candidate ({path.name}), gated by overnight_sweep\n\n"
                           "Automated; review the edit's semantics before keeping it.\n")
                    git("commit", "-q", "-m", msg)
                    wins += 1
                    log(f"  COMMITTED {git('rev-parse', '--short', 'HEAD')} {name} {verb}; now {measures(gate.report)}")
        finally:
            if src.read_text() != text and git("status", "--porcelain", "--", rel):
                src.write_text(text)    # (never leave an uncommitted candidate behind)
            current.clear()
        state[sym] = hashlib.sha1(src.read_text().encode()).hexdigest() if best is None else state.get(sym)
        state_path.write_text(json.dumps(state, indent=1))
    log(f"done: {wins} commits; {measures(gate.report)}")


if __name__ == "__main__":
    main()
