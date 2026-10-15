# MSL (Metrowerks Standard Library)

The C standard library the game was linked with. The headers and sources here come from the [Spirit Tracks decomp](https://github.com/zeldaret/st) (`libs/c`, commit `048ffeaa`), which is CC0. They're used unchanged.

Built without exception tables (`-Cpp_exceptions off`, see `LIB_CC_FLAGS` in `tools/configure.py`).
