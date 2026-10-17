# gapfuncs

dsd's function discovery stops scanning an overlay linearly at the first function it can't parse
(e.g. code that continues after a mid-function literal pool, or `bl` used as a long branch), and
afterwards only finds functions that are called directly. In this ROM that left ~150 KB of ov000,
ov006 and ov007 code without symbols or relocations (ov006/ov007 tails were even classified as
.rodata).

- `analyze.py OV` re-discovers Thumb functions in an overlay: seeds from thumb pointers in the
  overlay and calls, recursive descent per function (mwcc switch tables, literal pools, `bl`
  long branches), iterated until call targets stop adding seeds. It also re-checks dsd's
  functions and merges "functions" that are really long-branch labels of their predecessor.
  Results go to `build/gapfuncs/ovNNN.json`.
- `apply.py [--write] OV...` writes the functions, labels, relocations (calls, pool loads,
  vtable/table pointers in data) and new section bounds into `config/`. Targets without an exact
  symbol use dsd's `to:<symbol> add:<offset>` form.

Needs `capstone`. Verified by rebuilding: the ROM must still match.
