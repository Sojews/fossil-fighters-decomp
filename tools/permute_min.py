#!/usr/bin/env python3
"""Shrinks a permuter match to the edits it needs: drops diff hunks one by one while the function still matches.

Usage: python3 tools/permute_min.py src/foo.cpp SYMBOL build/permute/SYMBOL/best-0.cpp
Writes the minimal version over src/foo.cpp (check the result with try_match and the gate before committing).
"""
import difflib
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import permute  # noqa: E402


def matches(text, src, symbol, cmd, theirs):
    with tempfile.TemporaryDirectory(dir=permute.TMP) as tmp:
        p = Path(tmp) / src.name
        p.write_text(text)
        subprocess.run(cmd + ["-c", str(p), "-o", tmp + "/"], cwd=ROOT, capture_output=True)
        obj = Path(tmp) / (src.stem + ".o")
        f = permute.our_function(obj, symbol) if obj.exists() else None
        return f is not None and permute.score(permute.tokens(f[0], theirs[2], frozenset(f[1])), theirs[3]) == 0


def main():
    src, symbol, best = (ROOT / sys.argv[1]).resolve(), sys.argv[2], Path(sys.argv[3])
    a, b = src.read_text().split("\n"), best.read_text().split("\n")
    cmd = permute.compile_command(src)
    code, rel, thumb = permute.target(src, symbol)
    theirs = (code, rel, thumb, permute.tokens(code, thumb, frozenset(rel)))
    permute.TMP.mkdir(parents=True, exist_ok=True)
    hunks = [op for op in difflib.SequenceMatcher(None, a, b, autojunk=False).get_opcodes() if op[0] != "equal"]
    keep = list(range(len(hunks)))

    def build(sel):
        out, pos = [], 0
        for k, (tag, i1, i2, j1, j2) in enumerate(hunks):
            out += a[pos:i1]
            out += b[j1:j2] if k in sel else a[i1:i2]
            pos = i2
        return "\n".join(out + a[pos:])

    assert matches(build(keep), src, symbol, cmd, theirs), "the candidate doesn't match"
    for k in list(keep):
        trial = [x for x in keep if x != k]
        if matches(build(trial), src, symbol, cmd, theirs):
            keep = trial
    src.write_text(build(keep))
    print(f"kept {len(keep)} of {len(hunks)} hunks")


if __name__ == "__main__":
    main()
