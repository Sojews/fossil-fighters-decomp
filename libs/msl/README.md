# MSL (Metrowerks Standard Library)

The C standard library the game was linked with. The headers and sources here come from the [Spirit Tracks decomp](https://github.com/zeldaret/st) (`libs/c`, commit `048ffeaa`), which is CC0. They're used unchanged.

The original MSL objects were built with exception tables (their entries are in `.exceptix` at `0x02088498`). These files are built without them for now, so those entries stay in the unsplit data.
