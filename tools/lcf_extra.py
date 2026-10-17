#!/usr/bin/env python3
"""Adds the linker-defined constants the NitroSDK code refers to to dsd's linker script.

Usage: python3 tools/lcf_extra.py build/usa/arm9.lcf [other outputs...]

The SDK's own linker script defines symbols like SDK_IRQ_STACKSIZE whose addresses are sizes; the code takes
their addresses, so the values (as the game was linked) have to be defined here.
"""
import sys

SYMBOLS = {
    "SDK_IRQ_STACKSIZE": 0x800,
    "SDK_SYS_STACKSIZE": 0,
    # Overlay IDs (FS_OVERLAY_ID takes the address of SDK_OVERLAY_<name>_ID).
    **{f"SDK_OVERLAY_ov{i:03d}_ID": i for i in range(6)},
}

# Symbols the SDK's linker script puts at the start of the ARM9 .bss: the table of overlay digests (empty in this
# game, so both ends are the same address).
AT_BSS_START = ["SDK_OVERLAY_DIGEST", "SDK_OVERLAY_DIGEST_END"]

path = next(a for a in sys.argv[1:] if a.endswith(".lcf"))
text = open(path).read()
orig = text
lines = "".join(f"    {name} = {value:#x};\n" for name, value in SYMBOLS.items() if f"{name} =" not in text)
if lines:
    text = text.replace("SECTIONS {\n", "SECTIONS {\n" + lines, 1)
marker = "ARM9_BSS_START = .;\n"
extra = "".join(f"        {name} = .;\n" for name in AT_BSS_START if f"{name} =" not in text)
if extra and marker in text:
    text = text.replace(marker, marker + extra, 1)
if text != orig:
    open(path, "w").write(text)
