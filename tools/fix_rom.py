#!/usr/bin/env python3
"""Restore the two ROM fields dsd can't reproduce, then recompute their checksums.

Neither field is code; every code module is already checked byte-for-byte by `dsd check modules`.

1. Secure area CRC (header 0x6C). The stored value is a CRC of the *encrypted* secure area, and
   dsd needs the ARM7 BIOS to encrypt it. Without the BIOS it writes 0, so we copy it from the base ROM.
2. Banner icon bitmap. The icon palette has two identical colours (slots 3 and 5); dsd rebuilds the
   icon from a PNG and can't tell them apart, so a few pixels get the other slot. We copy the original
   bitmap and recompute the banner CRC.

Usage: fix_rom.py <base_rom> <built_rom>   (patches built_rom in place)
"""
import struct
import sys

SECURE_CRC = 0x6C
HEADER_CRC = 0x15E
BANNER_OFFSET = 0x68
BANNER_BITMAP = (0x20, 0x220)   # 4bpp 32x32 icon bitmap, relative to banner start
BANNER_CRC = 0x02               # CRC-16 of banner[0x20:0x840] for version 1 banners
BANNER_CRC_RANGE = (0x20, 0x840)


def crc16(data: bytes) -> int:
    crc = 0xFFFF
    for byte in data:
        crc ^= byte
        for _ in range(8):
            crc = (crc >> 1) ^ 0xA001 if crc & 1 else crc >> 1
    return crc


def main() -> None:
    base = open(sys.argv[1], "rb").read()
    built = bytearray(open(sys.argv[2], "rb").read())

    built[SECURE_CRC:SECURE_CRC + 2] = base[SECURE_CRC:SECURE_CRC + 2]

    banner = struct.unpack_from("<I", built, BANNER_OFFSET)[0]
    base_banner = struct.unpack_from("<I", base, BANNER_OFFSET)[0]
    lo, hi = BANNER_BITMAP
    built[banner + lo:banner + hi] = base[base_banner + lo:base_banner + hi]
    lo, hi = BANNER_CRC_RANGE
    struct.pack_into("<H", built, banner + BANNER_CRC, crc16(built[banner + lo:banner + hi]))

    struct.pack_into("<H", built, HEADER_CRC, crc16(built[:HEADER_CRC]))

    open(sys.argv[2], "wb").write(built)


if __name__ == "__main__":
    main()
