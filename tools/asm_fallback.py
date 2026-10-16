#!/usr/bin/env python3
"""Prints an mwcc `asm` function body for a game function, for functions that are decompiled but don't match yet.

Usage: python3 tools/asm_fallback.py <function name or 0xADDRESS>
Reads build/asm (run `tools/bin/dsd dis -c config/usa/arm9/config.yaml -a build/asm --ual --main` first) and
converts dsd's UAL syntax to the pre-UAL syntax mwcc's inline assembler accepts. Literal-pool loads become
`ldr rX, =value` (mwcc puts the pool at the end of the function, like the originals). Calls to C++ functions and
references to symbols need declarations in scope; mangled names need an `extern "C"` declaration. The names used
are printed as a comment. Thumb code isn't supported.
"""
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
COND = r"(eq|ne|cs|hs|cc|lo|mi|pl|vs|vc|hi|ls|ge|lt|gt|le|al)?"


def find(target: str) -> list[str]:
    for path in sorted((ROOT / "build/asm").rglob("*.s")):
        lines = path.read_text().splitlines()
        for i, line in enumerate(lines):
            m = re.match(r"^(\S+): ; (0x[0-9a-f]+)$", line)
            if m and target in (m.group(1), m.group(2)):
                body = []
                for l in lines[i + 1:]:
                    if re.match(r"^\S+: ; 0x", l) or "_func_end" in l:
                        break
                    body.append(l)
                return body
    sys.exit(f"{target} not found in build/asm")


def convert(line: str) -> str:
    s = line.split(";")[0].strip()
    s = re.sub(r"\.L_", "L_", s)
    if re.match(r"^\w+:$", s):
        return s
    if s.startswith((".word", ".byte", ".short")):
        sys.exit("literal pools aren't supported")
    m = re.match(rf"^push{COND} \{{(.*)\}}$", s)
    if m:
        return f"stm{m.group(1) or ''}db sp!, {{{m.group(2)}}}"
    m = re.match(rf"^pop{COND} \{{(.*)\}}$", s)
    if m:
        return f"ldm{m.group(1) or ''}ia sp!, {{{m.group(2)}}}"
    m = re.match(rf"^(lsl|lsr|asr|ror){COND}(s?) (\w+), (\w+), (#?\w+)$", s)
    if m:
        op, cond, flag, rd, rm, amount = m.groups()
        return f"mov{cond or ''}{flag} {rd}, {rm}, {op} {amount}"
    m = re.match(rf"^(ldr|str)(sb|sh|b|h){COND} (.*)$", s)
    if m:
        return f"{m.group(1)}{m.group(3) or ''}{m.group(2)} {m.group(4)}"
    m = re.match(rf"^(stm|ldm){COND} (\w+!?), (.*)$", s)
    if m:
        return f"{m.group(1)}{m.group(2) or ''}ia {m.group(3)}, {m.group(4)}"
    return s


def main() -> None:
    raw = [l for l in find(sys.argv[1]) if l.strip()]
    pool = {}
    for l in raw:
        m = re.match(r"^\.L_(\w+): \.word (\S+)$", l.strip())
        if m:
            pool[m.group(1)] = m.group(2)
    lines = []
    for l in raw:
        if re.match(r"^\.L_\w+: \.word ", l.strip()):
            continue
        m = re.match(r"^(\s*ldr\w* \w+), \.L_(\w+)$", l)
        if m and m.group(2) in pool:
            l = f"{m.group(1)}, ={pool[m.group(2)]}"
        lines.append(l)
    body = [convert(l) for l in lines]
    calls = sorted({l.split()[1] for l in body if re.match(r"^blx? \w", l)} | set(pool.values()))
    if calls:
        print(f"// uses: {', '.join(calls)}")
    print("    nofralloc")
    for l in body:
        if l:
            print(l if l.endswith(":") else f"    {l}")


if __name__ == "__main__":
    main()
