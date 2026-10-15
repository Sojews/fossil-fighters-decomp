# Fossil Fighters Decompilation

**Work in progress.** This project is recreating the source code of ***Fossil Fighters*** (Nintendo DS, 2009) by hand, so that it compiles back into a byte-identical ROM.

**This repository contains no game assets and no game code.** It holds hand-written C, symbol and address maps, and build scripts. To build, you need your own copy of the game: the build extracts everything else from it.

| | |
|---|---|
| Target | Fossil Fighters (USA), `YKHE`, SHA-1 `8148a65040b5085337b76e6051601da1b1fc28ef` |
| Functions | 10,076 found across ARM9 main, ITCM and 8 overlays |
| Matched | 2 (see `src/`) |
| Compiler | Metrowerks CodeWarrior for ARM 2.0 (`mwccarm`), NitroSDK 4.0 |

## Building

Works on macOS (Apple Silicon or Intel) and Linux; Windows should work but is untested.

1. Install Python 3.11+ and [Ninja](https://ninja-build.org).
2. Download the toolchain (dsd, objdiff, wibo and the Metrowerks compilers) into `tools/`:
   ```sh
   python3 tools/download_tools.py
   ```
3. Put your ROM at `extract/ff_usa.nds`.
4. Configure and build:
   ```sh
   python3 tools/configure.py usa
   ninja
   ```

`ninja` extracts the ROM, splits its code into objects, compiles everything in `src/`, links, checks every module byte-for-byte, builds `build/ff_usa.nds` and verifies its SHA-1. A clean build takes a few seconds.

Other targets: `ninja check` (module check only), `ninja objdiff.json` (config for [objdiff](https://github.com/encounter/objdiff) and decomp.me scratches).

### Why there's a `fix_rom.py`

Two non-code fields can't be regenerated without extra inputs, so they're restored from your base ROM after the build (see `tools/fix_rom.py`):

- The secure-area checksum is computed over the *encrypted* secure area, which needs the DS ARM7 BIOS.
- The banner icon's palette contains the same colour twice, and dsd's PNG round trip can't tell the two apart.

Every code module is checked independently by `dsd check modules`, so this doesn't hide code mismatches.

## How to contribute

1. Run `ninja objdiff.json`, then open the project in objdiff (or use the decomp.me scratch links it generates).
2. Pick a function, write C for it in a file under `src/`, and add the file's address range to `config/usa/arm9/delinks.txt` (see `src/util/string_trim.c` for an example).
3. `ninja` must still finish with `build/ff_usa.nds: OK`.

Tips:
- Most game code is Thumb; `-thumb` is the default. Wrap ARM functions in `#pragma thumb off`.
- `char` is signed in this game.
- The game contains C++ exception tables (`.exceptix`), so parts were written in C++.
- Include any alignment padding after the last function in a file's `.text` range, or later code shifts.

## Tools

- [ds-decomp](https://github.com/AetiasHax/ds-decomp) (`dsd`): extract, delink, linker script, ROM build and module checks
- [objdiff](https://github.com/encounter/objdiff): function diffing and progress reports
- [wibo](https://github.com/decompals/wibo): runs the Windows-only Metrowerks tools on macOS and Linux
- Build structure adapted from the [Phantom Hourglass decomp](https://github.com/zeldaret/ph) (CC0)

## License

The code and configs in this repository are released into the public domain under [CC0 1.0](LICENSE). This covers only what's in the repo; the game itself is not included and remains Nintendo's.

Fan project, not affiliated with or endorsed by Nintendo, Red Entertainment or Spike. Fossil Fighters is a trademark of Nintendo.
