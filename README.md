# Fossil Fighters Decompilation

**Work in progress.** This project is recreating the source code of ***Fossil Fighters*** (Nintendo DS, 2009) by hand, so that it compiles back into a byte-identical ROM.

**This repository contains no game assets and no game code.** It holds hand-written C, symbol and address maps, and build scripts. To build, you need your own copy of the game: the build extracts everything else from it.

| | |
|---|---|
| Target | Fossil Fighters (USA), `YKHE`, SHA-1 `8148a65040b5085337b76e6051601da1b1fc28ef` |
| Functions | 11,776 found across ARM9 main, ITCM and 8 overlays |
| Matched | 10,498 functions, 644,840 bytes (46.0%): the `M` engine library (tasks, interrupts, displays, BG layers, sprites, fonts, text, menus, pictures, 3D models and scenes, motions, resources, backups, wireless play), the NitroSDK, NitroSystem, MSL and MW runtime code the game uses, and much of the game: sound, events, the field, cleaning, saves, menus (incl. the shops, containers, team organization and the creature lists' sorts) and the battle models and UI |
| Named | 10,961 functions, incl. the NitroSDK/NitroSystem/MSL functions found by `tools/sdk_names` and the destructors and virtuals named from the RTTI |
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
2. Pick a function, write C for it in a file under `src/`, and add the file's address range to `config/usa/arm9/delinks.txt` (see `src/util/text_parse.c` for an example).
3. Check it with `python3 tools/try_match.py src/your_file.c` (it compares every function in the file with the game and shows a side-by-side disassembly of mismatches; `pip install capstone` for the disassembly).
4. `ninja` must still finish with `build/ff_usa.nds: OK`.

For a first draft of a function, run [m2c](https://github.com/matt-kempster/m2c) with `-t arm-mwcc-c` on its assembly from `dsd dis` (in UAL syntax, `--ual`).

Library code (NitroSDK, NitroSystem, MSL) lives at about `0x0206d000`–`0x02088364` and 762 of its 1,021 functions are already named. The game's own code (below `0x0206d000`, plus the overlays) is the main job.

The game keeps RTTI, so its real class names are known: see [docs/classes.md](docs/classes.md) (from `tools/rtti_names.py`, which also names every verified destructor).

Tips:
- Most game code is Thumb; `-thumb` is the default. Wrap ARM functions in `#pragma thumb off`.
- `char` is signed in this game.
- Parts of the game are C++ (the overlays have static initializers). Game code has no exception tables, so everything is built with `-Cpp_exceptions off`.
- C++ classes: mwcc 2.0 uses Itanium name mangling (`_ZN12SoundManager4InitEv`). Rename the game's symbols to the mangled names your code produces; base-class constructors/destructors are referenced as `C2`/`D2`, and if the game has one function for both variants, list both names at that address in `symbols.txt`. Pad unknown class members with `u32` arrays, not `u8`, so the class keeps its 4-byte alignment.
- A file whose statics need construction also owns its `.init` (the `__sinit_<file>` function), its `.ctor` slot and its `.bss`; see `src/sound/sound_manager.cpp`.
- If a function is right but won't match (usually a register swap), keep the C under `#ifdef NONMATCHING` and use an mwcc `asm` function with the original instructions in the `#else`, as in `SeqPlayerEx::RemoveEntry`. `python3 tools/asm_fallback.py <function>` prints the body in mwcc's syntax. Constructors that nothing calls (often the `C2` variant) are stripped by the linker, like in the original build.
- mwcc is picky about expression shape: `x *= 10; x += d;` and `x = x * 10 + d` compile differently, and indexing with `a[i++]` stops the loop being turned into pointer arithmetic.
- Include any alignment padding after the last function in a file's `.text` range, or later code shifts.
- The game was built with interprocedural analysis per file (`-ipa file`): its C++ files keep their statics, pointer-to-member constants and type infos in one block, sorted by size (a heapsort of the objects in reverse creation order), and address them as `block + offset`. The build doesn't use it by default, because it also lets mwcc inline functions defined later in a file, which breaks files that hold more than one original file. A file whose code reads that block needs `#pragma ipa file` at the top (see `src/m/region.cpp`); then the order of its static declarations decides the offsets. Where the block's order isn't reproduced yet, functions reach its pointer-to-member constants through an `extern "C"` struct laid over the block at the anchor the game uses (one member per slot, named after the function it points to; see `src/m/menu.cpp`).

## Tools

- [ds-decomp](https://github.com/AetiasHax/ds-decomp) (`dsd`): extract, delink, linker script, ROM build and module checks
- [objdiff](https://github.com/encounter/objdiff): function diffing and progress reports
- [wibo](https://github.com/decompals/wibo): runs the Windows-only Metrowerks tools on macOS and Linux
- [ntrtwl/NitroSDK](https://github.com/ntrtwl/NitroSDK), [ntrtwl/NitroSystem](https://github.com/ntrtwl/NitroSystem): reference builds for naming library functions (`tools/sdk_names`)
- MSL sources and headers from the [Spirit Tracks decomp](https://github.com/zeldaret/st) (CC0)
- Build structure adapted from the [Phantom Hourglass decomp](https://github.com/zeldaret/ph) (CC0)

## License

The code and configs in this repository are released into the public domain under [CC0 1.0](LICENSE). This covers only what's in the repo; the game itself is not included and remains Nintendo's.

Fan project, not affiliated with or endorsed by Nintendo, Red Entertainment or Spike. Fossil Fighters is a trademark of Nintendo.
