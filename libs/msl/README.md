# MSL (Metrowerks Standard Library)

The C standard library the game was linked with. The headers and sources here come from the [Spirit Tracks decomp](https://github.com/zeldaret/st) (`libs/c`, commit `048ffeaa`), which is CC0. Most are used unchanged. This game's MSL is a little older: its string and memory functions work on plain (signed) `char`, so `string.c`, `mem.c` and `mbstring.c` are adjusted for that, and the functions Spirit Tracks doesn't have (`strcat`, `strchr`, `wctomb`, `wcstombs`, `__memrchr`) are written here. `wstring.h` declares `mbstate_t` only for C++.

The original MSL objects were built with exception tables (their entries are in `.exceptix` at `0x02088498`). These files are built without them for now, so those entries stay in the unsplit data.
