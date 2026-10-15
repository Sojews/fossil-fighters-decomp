# SDK names

Names the Nintendo library functions in the game (NitroSDK, NitroSystem, MSL) by compiling the community decomps of those libraries and comparing the results with the game's code.

```sh
python3 tools/sdk_names/build_refs.py                                     # clone + compile references (~40s)
python3 tools/sdk_names/match.py tools/sdk_names/obj/* > build/sdk_names.tsv
python3 tools/sdk_names/apply.py build/sdk_names.tsv                      # rename func_*/data_* in symbols.txt
```

`match.py` masks out every byte a relocation touches (call targets, pointers) and looks for a game function with the same size and identical remaining bytes. Each match also names the functions and data it references. A match under 0x40 bytes only counts if another function from the same source file matches within 4 KB, because tiny functions look alike.

Findings that set the flags in `build_refs.py`:

- The game links the **Thumb** builds of NitroSDK and NitroSystem (`-thumb -DSDK_CODE_THUMB`). ARM builds only match the hand-written ARM functions.
- NitroSDK matches best with `mwccarm 2.0/sp1p2` without `SP1P3_BUG_FOR_CONDITIONAL_ASM_INSTRUCTIONS`, and NitroSystem with `2.0/sp2`. MSL matches as Thumb with the game's own compiler.
- The references are NitroSDK **4.2**, but the game shipped **4.0**, so many functions differ slightly and stay unnamed.

References (pinned in `build_refs.py`): [ntrtwl/NitroSDK](https://github.com/ntrtwl/NitroSDK) and [ntrtwl/NitroSystem](https://github.com/ntrtwl/NitroSystem), which are only compiled locally here and never copied into this repository because they have no license, and [zeldaret/st](https://github.com/zeldaret/st) (CC0).
