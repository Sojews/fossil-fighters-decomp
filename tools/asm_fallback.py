#!/usr/bin/env python3
"""Prints an mwcc `asm` function body for a game function, for functions that are decompiled but don't match yet.

Usage: python3 tools/asm_fallback.py <function name or 0xADDRESS>
Reads build/asm (run `tools/bin/dsd dis -c config/usa/arm9/config.yaml -a build/asm --ual --main` first) and
converts dsd's UAL syntax to the pre-UAL syntax mwcc's inline assembler accepts. Calls to C++ functions need an
`extern "C"` declaration of the mangled name; the needed ones are printed as a comment. Functions with literal
pools or Thumb code aren't supported.
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
    s = line.strip()
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
    m = re.match(r"^stm sp, (.*)$", s)
    if m:
        return f"stmia sp, {m.group(1)}"
    return s


def main() -> None:
    body = [convert(l) for l in find(sys.argv[1]) if l.strip()]
    calls = sorted({l.split()[1] for l in body if re.match(r"^blx? \w", l)})
    if calls:
        print(f"// calls: {', '.join(calls)}")
    print("    nofralloc")
    for l in body:
        print(l if l.endswith(":") else f"    {l}")


if __name__ == "__main__":
    main()
