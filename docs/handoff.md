# Handoff: ov001 / ov003 / ov004 / ov005

Snapshot taken 2026-11-02 at `origin/main` **0c602c57** (your HEAD). On that commit a fresh build plus
`objdiff-cli report generate` gives:

- **11503 / 11789** functions matched
- **81.60%** code matched, fuzzy 98.81%
- `build/ff_usa.nds: OK`

## 0. Who owns what

Two Claude sessions shared this repo. This document covers mine: **src/ov001, src/ov003, src/ov004 and src/ov005**.
The other session ("jonny-d6") owns src/game, src/m, ov000, ov002, ov006 and ov007. I don't know what it has in
flight. Before you edit those directories, or the shared headers `include/m/*`, `include/game/*` and
`libs/msl_cpp/include/*`, ask the user whether that session is still running.

Unmatched functions per area (from the report at 0c602c57):

| area | matched | unmatched | code % |
|---|---|---|---|
| ov001 | 439/464 | 25 | 68.0 |
| ov003 | 243/250 | 7 | 90.2 |
| ov004 | 964/976 | 12 | 89.2 |
| ov005 | 597/598 | 1 | 99.3 |
| *(other session)* game / m / ov000 / ov002 / ov006 / ov007 | | 46 / 58 / 46 / 24 / 15 / 13 | |
| libs msl / nitrosdk / nitrosystem / runtime | | 12 / 10 / 5 / 3 | |

## 1. Work in flight

**Nothing is uncommitted.** Every attempt is pushed. Each unmatched function below is real C, not a stub:
it compiles, objdiff scores it, and its unit links the original bytes (see §4, "partial units"). None of these
overlays uses `#ifdef NONMATCHING` with an asm fallback. Each function instead carries a `// NONMATCHING: <reason>`
comment above its definition.

Percentages are fuzzy match from `build/report.json`. "Line" is the definition, or the NONMATCHING comment just
above it.

### ov001

| % | size | function | file:line | remaining mismatch |
|---|---|---|---|---|
| 99.99 | 472 | `DinoField::LoadSlideInPict` | dino_field.cpp:1025 | pool offset: the static `sDirs` sits at a different offset in the TU's data, because not all of the game's data before it is emitted here |
| 99.93 | 492 | `DinoFieldCamera::IdleUnk2` | dino_field_camera.cpp:437 | stack slots: the setting's scale vs the easing temps |
| 99.86 | 2222 | `DinoField::ResolveMove` | dino_field.cpp:343 | the game's call to MFx32Point's implicit copy ctor resolves to an in-file copy; ours goes to a weak copy. Needs the game's ipa emission order |
| 99.81 | 284 | `DinoFieldRaster::Idle` | dino_field_raster.cpp:31 | stack slots of dead temps |
| 99.59 | 1352 | `func_ov001_0215f900` | dino_field_map.cpp:228 | register allocation |
| 99.37 | 1480 | `DinoFieldCaptureFadeOutTask::Idle` | dino_field.cpp:1864 | register swap: the smart OAM is in r6 in the game, r7 in ours; the game also re-tests it after `new` |
| 98.84 | 840 | `DinoFieldTownMap::DinoFieldTownMap` | dino_field_town_map.cpp:53 | the `this + i*4` family (§6) |
| 98.82 | 564 | `DinoFieldChara::IdlePatrol` | dino_field_chara_angle_anime.cpp:872 | registers of an s64 squared-length sum |
| 97.84 | 108 | `DinoField::WalkOnLand0` / `WalkOnLand1` | dino_field.cpp:1551 | one instruction: the game re-materializes `movs r2,#0`, ours reuses the loaded byte |
| 97.42 | 296 | `DinoField::SlideOutPict` | dino_field.cpp:1078 | pool offset of `sDirs` (same cause as LoadSlideInPict) |
| 97.26 | 716 | `DinoField::Step` | dino_field.cpp:1732 | the game builds `MFx32(1)` directly into `operator+=`'s parameter, while ours keeps a temp (+4 slot shift); the game also stores the rotation after computing its y |
| 96.54 | 604 | `DinoFieldChara::IdleWander` | dino_field_chara_angle_anime.cpp:838 | the random distance gets its own stack slot across `Next()` |
| 96.49 | 352 | `DinoField::ZoomOutPict` | dino_field.cpp:1157 | stack layout of the scale points, plus a cached ease |
| 95.62 | 536 | `DinoField::LoadZoomInPict` | dino_field.cpp:1095 | build order of the scale points, a cached ease, and LoadFadeInPict's slots |
| 95.62 | 1076 | `DinoFieldChara::Unk02162154` | dino_field_chara_angle_anime.cpp:722 | the game's frame is 32 bytes smaller and its registers differ |
| 95.45 | 500 | `DinoFieldChara::IdleFollow` | dino_field_chara_angle_anime.cpp:795 | registers: the s64 high word is in r4 and the defs in r6 in the game |
| 95.28 | 288 | `DinoFieldBattleStartEffect::StateClose` | dino_field_battle_start_effect.cpp:50 | the game re-takes the pooled min-radius address for `std::max`'s result |
| 90.61 | 1652 | `DinoFieldCamera::UnkV14` | dino_field_camera.cpp:224 | the game truncates yaw and pitch to u16 two more times (more inline levels?), and the drag temps are scheduled differently. A real expression-shape job |
| 90.52 | 832 | `DinoFieldCamera::IdleUnk0` | dino_field_camera.cpp:314 | the game's frame has 32 more bytes of temps and reads the first distance after the radius |
| 89.47 | 168 | `DinoField::FindRockHole` | dino_field.cpp:1673 | the game hoists `&hole->pos` and both halves of `pos` out of the loop. A `near` hoist plus `#pragma opt_loop_invariants off` reached 91.2 locally but was not committed |
| 87.04 | 372 | `MFx32Rect::PushOut` | include/m/fx32.h:243 | weak copy emitted into dino_field. **Shared header** |
| 86.38 | 68 | `operator+(const MFx32Rect&, MFx32Point)` | include/m/fx32.h:217 | the game stores one operand copy per add. **Shared header** |
| 86.12 | 6968 | `DinoFieldPlayerControl::Idle` | dino_field_player_control.cpp:137 | stack temp layout throughout. Biggest byte count left |
| 66.18 | 3352 | `DinoFieldRadar::Idle` | dino_field_radar.cpp:286 | same calls in the same order, but the inline math leaves about 20 extra temps (frame 0x660 vs 0x5DC), which pushes much of it past sp+0x3FC. Lowest % |

### ov003

| % | size | function | file:line | remaining mismatch |
|---|---|---|---|---|
| 99.79 | 120 | `__sinit_dino_ui_revive_status.cpp` | dino_ui_revive_status.cpp:1164 | under ipa, the two rects' dead temps are laid out per rect, while the game interleaves them. It matched in a non-ipa build, but the file needs ipa. Splitting it into one object per line gives 99.89 |
| 98.85 | 1158 | `DinoUiReviveStatusParts` ctor | dino_ui_revive_status.cpp:1042 | `this+i*4` family, plus two DinoCreatureState temps in swapped slots |
| 98.37 | 760 | `DinoUiReviveStatusDigit` ctor | dino_ui_revive_status.cpp:996 | `this+i*4` family |
| 97.81 | 572 | `DinoUiFieldMenuFraction4` ctor | dino_ui_field_menu_fraction.cpp:1002 | `this+i*4` family |
| 96.35 | 368 | `DinoUiPartsTeamTech` ctor | dino_ui_field_menu_fraction.cpp:407 | `this+i*4` family |
| 94.61 | 1140 | `DinoUiRvInformation::ShowAttackPage` | dino_ui_rv_information3d_model.cpp:453 | ours strength-reduces the attack loops, and the row search keeps `n` on the stack. `opt_strength_reduction off` and a guarded loop both score worse |
| 94.12 | 212 | `DinoUiFieldMenuNumber` ctor | dino_ui_field_menu_fraction.cpp:113 | `this+i*4` family |

### ov004

| % | size | function | file:line | remaining mismatch |
|---|---|---|---|---|
| 99.99 | 974 | `DinoUiCannonParentScanner::StateStart` | dino_ui_wireless_cancel_item.cpp:1017 | under ipa, two spill slots (the show transfer and `mTens`) are swapped |
| 99.99 | 624 | `DispBlower::Execute` | disp_blower.cpp:52 | the wait message's position temp sits lower on the stack in the game |
| 99.93 | 544 | `DinoUiDebugReviverMenu::Refresh` | dino_ui_debug_reviver_menu.cpp:92 | stack slots of the inlined item ctor |
| 99.78 | 784 | `DinoUiDebugReviverMenu::OnIdle` | dino_ui_debug_reviver_menu.cpp:39 | stack slots and registers (the item is in r6 in the game) |
| 99.52 | 836 | `DinoUiIslandMapMenu::OnFocus` | dino_ui_island_map.cpp:393 | the game loads both spot-table addresses first, so x and y swap registers. `MPoint(0,0)-p` made no difference |
| 99.21 | 1708 | `DinoUiSaveSlotConfirm::Unk02159590` | dino_ui_top_menu.cpp:1032 | the summary sits below the inlined item's locals, and item/oam swap registers |
| 98.75 | 2420 | `DinoUiEnding::StateStart` | dino_ui_top_menu.cpp:1248 | scheduling only: the game loads the strip position before storing the rect constants |
| 98.24 | 172 | `DinoUiWirelessCancel::CreateCloseTask` | dino_ui_wireless_cancel_item.cpp:791 | register swap between the sound task and the set |
| 95.23 | 556 | `DinoUiExchange::StateSave` | dino_ui_exchange.cpp:347 | the game's `set::insert` leaves two empty temps on the stack (zeroed after the call), which shifts the slots |
| 92.23 | 3188 | `DinoUiDebugKasekiRockGetMenu::OnStart` | dino_ui_debug_kaseki_rock_get.cpp:303 | register and slot choices in the item ctors (centring halves, the type items' oam) |
| 89.92 | 544 | `__sinit_dino_ui_island_map.cpp` | dino_ui_island_map.cpp:626 | same stores in the same order, but anchors and constants are in other registers |
| 79.00 | 70 | `DinoUiDebugFlagMenuItem::GetFrame` | dino_ui_debug_flag_menu.cpp:50 | the game computes right and bottom before storing any edge, and keeps 16 in r1. Small and not seriously attacked yet |

### ov005

| % | size | function | file:line | remaining mismatch |
|---|---|---|---|---|
| 79.51 | 424 | `DinoUiCleaningMenu::Decide(int)` | dino_ui_cleaning_menu_item.cpp:498 | the two column tests must not CSE — `(u32)(index % 4) == 3` on the Y axis does that (both match as cmp/bne now). The game computes the Y column product (r0) before building X; ours builds X first. Pure scheduling/register choice left |

## 2. Frontier

- **Nothing in the overlays is left to delink.** Every overlay's `.text` is fully covered by `src/` ranges in
  `config/usa/arm9/overlays/ov00N/delinks.txt`. I checked by computing the gaps. Main arm9 has only two tiny
  uncovered spans: `0x02000b64–0x02000ba0` and `0x0208638c–0x020863ac`. The work left is matching the ~286
  unmatched functions, not adding files.
- The last delinks changes in my overlays were on 2026-10-23: `__sinit_dino_ui_island_map.cpp` (87751caa) and
  `__sinit_disp_blower.cpp` (ac216e6c). Since then every change in this area has been C edits only.
- Overlay base: ov000–ov005 load at `0x021558a0`; ov006 and ov007 load at `0x02171b00`.

**What I'd attack next, in this order:**

1. *(Done: PR #1's two library functions were merged in the same commit as this file, so main is now 11505.)*
2. **`DinoUiCleaningMenu::Decide`** (ov005, 79.5%). It's the last unmatched function in ov005; see the table row for
   the current state. The >77% moves came from keeping both column tests (CSE-blocks them: `(u32)(index % 4) == 3` on the Y axis; both now cmp/bne). Hoisting row terms or separate locals made it worse.
3. **`DinoUiDebugFlagMenuItem::GetFrame`** (70 bytes, 79%). Compute right and bottom into locals before
   storing any edge.
4. **Low-percentage expression-shape rewrites with large byte counts:** `DinoFieldRadar::Idle` (66%, 3.3 KB),
   `DinoFieldCamera::UnkV14` / `IdleUnk0` (~90%), `DinoFieldPlayerControl::Idle` (86%, 7 KB). These move code %
   the most. Most of the gap is too many inline temporaries. Try fewer named locals, temps built inside static
   inline helpers, by-value accessors, and `MPoint(0,0) - p` forms.
5. **Leave the 99.x register/slot puzzles alone** unless you have a new idea. Every sweep and a 600 s permuter
   round on each failed to move them (§6).

## 3. Uncommitted state

In my checkouts at 0c602c57:

```
$ git status --short
?? .venv            # local symlink to the Python venv, not part of the repo
$ git diff --stat
(empty)
```

There's nothing worth keeping, so there is no `docs/handoff.patch`.

## 4. Conventions not in the README

**Build and report mechanics**

- **Partial units.** A `src/` file listed in `delinks.txt` *without* a `complete` line links the **original
  bytes**, but is still compiled so objdiff can score it (see `tools/configure.py`, the `partial` list). That's
  why non-matching C can't break the ROM. Main arm9 has 116 `complete` units; **no unit in ov001–ov005 is marked
  `complete`, even the ones at 100%** (for example `src/ov005/dino_ui_shop_sell.cpp`, 77/77). I never tested
  flipping them, so don't assume it's free: overlay data/bss ownership may not be reproduced. Run a full `ninja`
  and check for `OK` if you try.
- **`build/report.json` is the truth.** Generate it with `tools/bin/objdiff-cli report generate -o build/report.json`.
  `objdiff-cli diff` on a single symbol can disagree with it in either direction when a call goes to a template copy
  that the game keeps in another unit, or to the next symbol. Judge progress by the report.
- **No-regression rule before every push:** no function that was at 100 may drop. Compare against a report
  generated before the change.
- Builds are incremental. `tools/fix_dep.py` strips wibo's `Z:/` prefix from `.d` files; without it, every build
  recompiles ~400 files.

**Symbols**

- **Symbol size bugs.** If the game has non-zero bytes between a function symbol's end and the next symbol, the
  size in `symbols.txt` is probably short. DinoReplaceTag was 0x66 instead of 0x68, because its trailing `pop`
  was outside the symbol. Every hit repo-wide is now fixed except `func_ov007_02182020`, which is 2 bytes short
  but which our code can't match yet.
- C++ symbols are renamed in `symbols.txt` to the mangled names we produce. Both ctor variants share one address
  where the game merged them.

**ipa and static data**

- **`#pragma ipa file` per TU.** The game used `-ipa file`. Under ipa:
  - static declaration order decides offsets in the TU's pooled block;
  - unreferenced statics are dropped;
  - functions defined later in the file can be inlined.
- **Sinit pools** (`include/game/dino_static_init.h`):
  - `DINO_POOL_WORD` is volatile, so the store order is fixed.
  - `DINO_POOL_NV` is non-volatile, so the scheduler may reorder stores. Use NV for the first N stores in offset
    order when the game interleaves them (this matched the wireless_cancel sinit).
  - `DINO_COMMON_LAYOUT_INIT` / `DINO_COMMON_LAYOUT_INIT2` / `DINO_POOL_AT`: pool offsets ≥ 0x80 go through a
    second anchor, the data symbol at pool+0x80, which is loaded first.
- **Pooled bool constants for `set::insert_unique`.** The offsets differ per TU: in the exchange TU, true is at +1
  and false at +2; text_codec has the reverse. Use an explicit specialization that reads them from the pool
  symbol, with the pool struct declared above the tree instantiation (`src/ov004/dino_ui_exchange.cpp`).

**Stack slots and registers**

- **Local placement.** Named locals go top-down; compiler temps go below them; temps from inline helpers go
  lowest. **Named locals inside an inline are laid out ascending.** To get the game's descending order, below
  the caller's locals, pass the values as the caller's expression temporaries through `const&` params. Example:
  `PickExchangeId(gDinoResource->GetExcavateDefs(), gDinoResource->GetExcavateDefs())` matched MakeExchangeEntry.
- **Spill slots.** Among values live across calls, ours spills the value with the fewest uses first, into the
  lowest slot. To flip two slots, change their relative use counts, for example by re-reading a member at one use
  instead of going through a local.
- **A pointer the game reuses across an if/else-if chain** under `opt_common_subs off` should be one named local
  over the whole chain (`MInput *input = mInput;` in RvInformation3DModel::Idle).
- **Loop-top cached member address:** `MTaskSet *parallel = SomeNonMemberStaticInline(this);` where the inline
  returns `&obj->mParallel` (DinoFieldDance::Start, DinoUiPartsKasekiButton::Setup).
- **Point copied field by field from another object's MPoint member:** use the by-value accessor
  (`MPoint font = mMarkMove->GetValue();` matched SpotMove::Idle).
- **One shared local across two branches**, declared at the top, instead of one per branch (OnIdle +0.2).

**Expression shape**

- `x += y` and `x = x + y` compile differently for MFx32, and so do `a *= s` and `a = a * s`. Try both
  (`KasekiRock::StateDrop`/`Hit` matched this way).
- Compare-operand order is canonicalized, so swapping `a == x` does nothing. However,
  `u32 next = m84; if (m80 != next)` did fix the cmp operand order.
- **To stop mwcc CSE-ing two identical comparisons, change one side's type, not its value:
  `(u32)(index % 4) == 3` instead of a second `index % 4 == 3`.** Same `cmp/bne` bytes, different
  expression. (DinoUiCleaningMenu::Decide 77.20→79.51 — the game's two column tests both emit.)
- **Permuter outputs: keep the no-op-looking casts** (`(u32) w`, `(int) x`). They're often what matters. Remove
  them one at a time, never all at once.
- **Member read twice around a post-increment and then stored:**
  `int next = x + 1; int prev = *(volatile int *)&x; *(volatile int *)&x = next;` Both volatiles are needed.
- `MPoint(0, 0) - p` instead of `MPoint(-p.x, -p.y)` keeps a component in a register across a call.

**Commits**

- Message style: `<Function> X->Y: <idiom that did it>`, or `Match <Function> (<idiom>)`, with
  `git pull --rebase` before pushing to `main`.

## 5. Tooling

The symptom-organized matching playbook is [docs/matching-idioms.md](matching-idioms.md); it also documents
`tools/overnight_sweep.py` (unattended permuter + gate + commit) and `tools/callcheck.py` (call-target check).

Use **`.venv/bin/python`** for the permuter and try_match; they need capstone, and the venv has 5.0.7.

- **try_match:** `.venv/bin/python tools/try_match.py src/ov001/dino_field.cpp _ZN9DinoField12FindRockHoleE10MFx32Point`.
  It filters by **mangled** name; omit the name for every function in the file. It's fine for a first look. For scoring I used the objdiff
  helpers below, because they match the report.
- **Single-function permuter.** I ran it in a separate worktree (a git worktree synced to `origin/main`) so it
  never touches my edit tree:
  ```sh
  .venv/bin/python tools/permute.py src/ov001/dino_field.cpp \
      _ZN9DinoField12FindRockHoleE10MFx32Point "DinoFieldHole *DinoField::FindRockHole(" -t 600 -j 3
  ```
  - The third argument is a literal prefix of the definition line.
  - Results go to `build/permute/<SYMBOL>/best-<score>.cpp`.
  - Use `--resume` to continue from the best result, and `--no-helpers` to leave static inlines alone.
  - I used `-j 3` per queue with two queues side by side, the default random seed, and 600 s per function.
  - **The permuter score does not track objdiff %.** A drop from 293 to 244 can be worth 0.00% in the report.
    Re-score every candidate with objdiff before applying it.
- **Batch permuter:**
  `.venv/bin/python tools/permute_batch.py --min 95 --units src/ov001 src/ov003 -t 600 -j 3`.
  - It reads `build/report.json`, so generate the report first.
  - It writes a summary to `build/permute/batch.log` and changes nothing in `src/`.
  - `--list` prints the candidates. `--improved LOG` re-runs only the functions that improved, starting from
    their best result.
- **Helper scripts from my machine, not in the repo.** Run them from the repo root. The core three are in the
  appendix.
  - `fs.py UNIT SYM...`: rebuilds one object (`ninja build/usa/<unit>.o`) and prints objdiff's match % per
    symbol. This is the fast inner loop.
  - `vt.py SPEC.py`: variant tester. SPEC defines `FILE`, `UNIT`, `SYMS` and
    `VARIANTS = [(name, old, new) | (name, [(old, new), ...])]`. It tries each variant, scores it with fs.py,
    and always restores the file.
  - `dk.py UNIT SYM`: prints only the rows objdiff itself marks as differing (`diff_kind`). Counting raw differing
    rows is noisy, because branch targets are formatted differently on the two sides.
  - `gapscan.py REPORT`: the symbol-size check from §4.
  - `vers.sh`: compiles one TU with all ten mwcc 2.0 builds (`base` … `sp2p4`) and scores it. **Result: no
    version changes anything in these overlays.** It's here so you don't repeat it.
  - Regression check: generate a report before and after, then list every function that was 100 before and
    isn't after. It's a ten-line Python script over `units[].functions[].fuzzy_match_percent`.

## 6. Open questions, dead ends, PR #1

**PR #1 (totomam, "A small gift of functions from the Fossil Fighters: Champions decomp").** I reviewed it
against 0c602c57:

- **3 of its 5 changes are obsolete.** `MChr256Filter::Filter`, `MBackupStorage::AllocRegion`, and
  `MCanvas::ReadRow`/`WriteRow` are already at 100% on main, matched independently, so those hunks
  (`src/m/obj.cpp`, `src/m/backup.cpp`, `src/m/canvas.cpp`) **conflict**.
- **The other 2 apply cleanly and match.** These are `LoadSingleWave` (91.66 → 100) and `func_020878e8`, the EH
  HandleUnexpected (99.17 → 100). I tested:
  ```sh
  gh pr diff 1 -R Sojews/fossil-fighters-decomp > /tmp/pr1.diff
  git apply --include='libs/*' /tmp/pr1.diff
  ninja && tools/bin/objdiff-cli report generate -o build/report.json
  ```
  Result: 11503 → **11505**, no regressions, `build/ff_usa.nds: OK`.
- **Merged (only the two `libs/` files)** in the commit that added this handoff, with the author credited.
  The PR itself is still open: replying on it and closing it is the user's call.

**Traps. Don't spend time here without a genuinely new idea:**

- **The `this + i*4` family.** The game keeps a digit or icon slot address (`this + i*4`) in a stack slot across
  the path `std::string`'s destructor; ours recomputes it. This affects the FieldMenuNumber, Fraction4, TeamTech,
  ReviveStatusDigit/Parts and TownMap ctors (5–6 functions). All of these produced identical output:
  - all ten mwcc 2.0 versions;
  - ipa on or off;
  - opt pragmas;
  - reference or pointer slot locals (the address is kept, but hoisted as `this+0x74`);
  - chained assignment;
  - helper functions;
  - type changes;
  - the permuter.

  Cracking it would probably fix all of them at once, but it's the deepest hole here.
- **`DINO_COMMON_LAYOUT_INIT` sinits:** mixing NV and volatile pool stores changes nothing, because the
  single-anchor aliasing blocks the game's reordering.
- **`LoadSlideInPict` / `SlideOutPict`:** the `sDirs` bss/data offset depends on game data that this TU doesn't
  emit.
- **`ResolveMove`:** needs the game's ipa emission order (a `bl` to a local copy-ctor emitted right after it).
- **Permuter-dead.** These didn't move objdiff % in a 600 s round: OnFocus, CreateCloseTask, DispBlower::Execute,
  IdlePatrol/IdleFollow (s64 sum registers), StateClose, DebugKasekiRockGet::OnStart, ShowAttackPage, and the
  revive_status sinit.
- **Permuter "improvements" I mined that didn't help:** LoadZoomInPict (110 → 81 score), the TownMap ctor
  (144 → 114), Fraction4 (142 → 112; noise output that even reads a variable before setting it), and IdleUnk2
  (+0.01%). Untested: the ReviveStatusParts candidate (293 → 244, a flag copy and `level != 0`).
- **`MFx32Rect::PushOut` and `operator+`** are inline in the shared `include/m/fx32.h`. Changing them affects every
  TU that uses them, so coordinate before touching them.

**Suspected wrong, not verified:**

- `func_ov007_02182020` (other session's area): symbol size looks 2 bytes short (0x12ce, probably 0x12d0).

## 7. Build reality

- A clean `python3 tools/configure.py usa && ninja` at 0c602c57 finishes with **`build/ff_usa.nds: OK`** on macOS
  (Apple Silicon, wibo + mwccarm 2.0).
- The default target is `rom partial`. `rom` is the SHA-1 check, and `partial` compiles the non-`complete` units
  for objdiff. `ninja check` runs the module check only.
- There are no known-broken files. Any file can be edited: non-matching C in a partial unit can't break the ROM.
  It can only lower the report.
- Every unit uses the default compiler version except `libs/nitrosdk/src/crypto/`, which uses mwcc 1.2/sp2 and is
  set in `CC_VERSION_BY_PATH` in `tools/configure.py`.
- Linux: the build is meant to work there, but I've only run it on macOS.

---

## Appendix: core helper scripts

Save these anywhere and run them from the repo root.

```python
# fs.py UNIT SYM [SYM...]   e.g. fs.py src/ov001/dino_field _ZN9DinoField12FindRockHoleE10MFx32Point
import json,subprocess,sys
u=sys.argv[1]
r=subprocess.run(['ninja','build/usa/'+u+'.o'],capture_output=True,text=True)
if r.returncode: print('BUILD FAIL'); print('\n'.join(l for l in r.stdout.split('\n') if 'rror' in l)[:1500]); sys.exit(1)
for sym in sys.argv[2:]:
    d=json.loads(subprocess.run(['tools/bin/objdiff-cli','diff','-p','.','-u',u,'-o','-','--format','json',sym],capture_output=True,text=True).stdout)
    L=[s for s in d['left']['symbols'] if s.get('name')==sym and s.get('instructions')]
    R=[s for s in d['right']['symbols'] if s.get('name')==sym and s.get('instructions')]
    if not R: print(sym,'NOT FOUND in ours'); continue
    print('%.3f'%L[0].get('match_percent',0), sym)
```

```python
# vt.py SPEC.py : SPEC defines FILE, UNIT, SYMS, VARIANTS=[(name,old,new) | (name,[(old,new),...])], optional ALL=True
import sys,subprocess,runpy,os
FS=os.path.join(os.path.dirname(os.path.abspath(__file__)),'fs.py')
sp=runpy.run_path(sys.argv[1]); FILE,UNIT,SYMS,V=sp['FILE'],sp['UNIT'],sp['SYMS'],sp['VARIANTS']; ALL=sp.get('ALL',False)
orig=open(FILE).read()
score=lambda:' | '.join(subprocess.run(['python3',FS,UNIT]+SYMS,capture_output=True,text=True).stdout.strip().split('\n'))
try:
    print('%-14s %s'%('base',score()))
    for v in V:
        name=v[0]; edits=v[1] if isinstance(v[1],list) else [(v[1],v[2])]; s=orig
        if any((s.count(o)!=1 and not ALL) or s.count(o)==0 for o,_ in edits): print(name,': old text not unique/found'); continue
        for o,n in edits: s=s.replace(o,n)
        open(FILE,'w').write(s); print('%-14s %s'%(name,score()),flush=True)
finally:
    open(FILE,'w').write(orig); subprocess.run(['ninja','build/usa/'+UNIT+'.o'],capture_output=True)
```

```python
# dk.py UNIT SYM : only the rows objdiff marks as different (left = game)
import json,subprocess,sys
u,sym=sys.argv[1],sys.argv[2]
d=json.loads(subprocess.run(['tools/bin/objdiff-cli','diff','-p','.','-u',u,'-o','-','--format','json',sym],capture_output=True,text=True).stdout)
pk=lambda s:[x for x in d[s]['symbols'] if x.get('name')==sym and x.get('instructions')][0]['instructions']
l,r=pk('left'),pk('right')
for i in range(max(len(l),len(r))):
    a=l[i] if i<len(l) else {}; b=r[i] if i<len(r) else {}
    k=a.get('diff_kind') or b.get('diff_kind')
    if k and k!='DIFF_NONE': print(i,k,a.get('instruction',{}).get('formatted'),'|',b.get('instruction',{}).get('formatted'))
```

---

## Session update 2026-11-08 (a second agent pass; same four-overlay area)

State at origin/main **acb44c50** (report: 11633/11787 fuzzy-100). Changes this pass, all committed and pushed, no report regressions:

- **DinoUiDebugFlagMenuItem::GetFrame 79.00 -> 88.17** (`MPoint pos = mPos;` before building the MRect — the local copy fixes the schedule: 16 lands in r1 instead of a 4th saved reg, right/bottom adds emit before the edge stores). Tested and rejected: folded 4-edge ctor (69.7), MPoint-pair ctor (44.2), ref-to-member (79.0), direct-mPos ctor (64.4). Remaining 9 rows: y-before-x load order, one extra spill slot. A 600s permuter round produced nothing (base 54 = best 54).
- **DinoFieldCamera::UnkV14 90.61 -> 90.94**: orbit yaw/pitch params widened u16 -> int, with `(u16)pitch` on the first clamp compare and `(u32)pitch` on the second + both else-if compares. The (u32)s are identity-in-code but unsigned (blo/bhs like the game) and AST-distinct so they don't merge with the (u16) cast. Blanket (u16) everywhere regresses (90.45); the game's second unmerged trunc pair (lsl/lsr, lsl/lsr at the clamp) is still 2 instructions off.
- **DinoFieldBattleStartEffect::StateClose 95.28 -> 96.60**: `std::max((int)data_ov001_0216aad4.mMinRadius, radius)` — the (int) cast makes mwcc fresh-reload the pool member on the max's result path instead of reusing the cached register. Explicit-if and ternary forms regress (~84-87); `std::max(radius, min)` slightly worse (95.15). One spill slot (sp+0x14) still differs from the game's pool re-load.

Dead ends confirmed this pass (files restored): Decide permuter best-383 scores 78.8 vs 79.5 (permuter score ≠ report again); WalkOnLand0/1 literal-0 collapses to 56.7, `water != 0` to 92.2 — the game's `movs r2,#0` is allocator-order, permuter-dead too (best 12 = base 12 after 408 compiles); DinoUiExchange::StateSave pair-temp / mac-local shapes don't produce the game's two zeroed temps (all ~95.1); DinoUiEnding::StateStart DinoEndingStrip folded-ctor and pos-first variants regress (95.6 / 97.5) — the "loads strip position before rect constants" scheduling stands, and sp offsets shift by ~0x10 in the mid-function.

Still open in these overlays (unchanged): DinoFieldRadar::Idle 66.18, DinoFieldPlayerControl::Idle 86.12, DinoFieldCamera::IdleUnk0 90.52 (game's frame is 40 bytes BIGGER — needs extra temps, not fewer), DinoUiDebugKasekiRockGetMenu::OnStart 92.23, DinoUiEnding::StateStart 98.75, the `this+i*4` family, and the 99.x slot puzzles. Frontier order stands: low-% large-byte rewrites first (Radar, PlayerControl, IdleUnk0), then small surgical (saved for permuter runs).

---

## Session update 2026-11-08 (third agent pass; same four-overlay area)

On top of the pass above (HEAD 146461b5). Committed and pushed, no report regressions:

- **DinoFieldCamera::IdleUnk0 90.52 -> 92.15** (a 600s permuter round found it — the round's own score dropped 674->555 while the report ROSE, the permuter-score ≠ report mismatch again). IdleUnk0's "game frame 40 bytes BIGGER" turned out to mean: materialize every used value into its own named local (`camera`/`def`/`circle`/`inner`/`outer`/`len` declared in the game's order at the top, per-value slot copies like `u32 perm578 = camera.x` before the store, `MPoint perm437` before ToFx32Point), plus `0 == x` compare forms and the comparison as `MFx32(circle->radius).v < d.v`. Dummy/never-read `perm` locals from the permuter were removable for free (92.151 with or without). The two point temps for `(mFocusPos - c)` and `(mFocusPos - center)` are still in different slots than the game.

Dead ends confirmed this pass: PlayerControl::Idle's follower loop (the game keeps `i` and recomputes `field + i*4` per iteration, no strength reduction; ours keeps a running pointer) — `#pragma opt_strength_reduction off` regresses to 85.55, `opt_loop_invariants off` to 85.88, `opt_common_subs off` to 82.87, inline-loop/GetCharaSlot/GetChara variants unchanged or slightly worse (the game SR'd its other loops, so the global pragma is too blunt). Re-scored via `objdiff-cli diff` per unit (report format no longer carries per-symbol data).

---

## Session update 2026-11-08 (third pass, continued)

On top of the pass above. Committed and pushed, no report regressions:

- **DinoFieldCamera::UnkV14 90.94 -> 91.88** (dc58b74b): another 600s permuter round (own score 906->686 while the report ROSE — the score-≠-report mismatch again). The whole-file candidate was NOT usable as-is: its mutation of the shared `GetExcavate78` helper broke IdleUnk2 (99.93 -> 97.17) and regressed IdleUnk0 to 90.5. Transplanted only UnkV14 + DinoCameraDamp/DinoCameraOrbit (private to UnkV14) onto the pushed state, and removing the candidate's `perm156/perm659` ease locals IMPROVED it to 91.880. Lesson: score whole-file candidates for every sibling function before adopting, and transplant per-function.
- **DinoFieldRadar::Idle 66.17 -> 66.75** (2d5360de): permuter round (own score 10115->9339, report +0.58). Winning shape: `entry`/`side`/`mtx` hoisted to the function top, `p` computed before the NULL check, `direction`/`near1`/`angle` split from their init (`int angle = mPlayerAngle` + separate `near1 =`), shared `d` + assigned `side` in the range loop, `MFx32Point d` hoisted out of DinoRadarSwept's block. Most `perm` locals were removable for free; the tail468/angle-near1 pair is load-bearing (removing it alone drops to 66.61).

Dead ends this stretch: DinoUiDebugReviverMenu::OnIdle shared-`state` hoist regresses (99.735->99.707, item's r6/r7 allocation untouched); OnFocus combined-expr/y-first flat or slightly worse (99.52, still permuter-dead); CreateCloseTask se-first/set-first reorders regress hard (87.1/85.3); DinoUiTopMenu ctor's only mismatch is a `.word` vtable data constant (99.815).

---

## Session update 2026-11-08 (third pass, round 2)

All four permuter targets landed, committed and pushed, no report regressions:

- **DinoFieldPlayerControl::Idle 86.10 -> 87.33** (0521e743): permuter round (own score 7067->6533, report +1.22). Winning shape: `follower`/`speed`/`ground`/`area`/`decide`/`tapped`/`dist` hoisted to the function top, `speed = walkSpeed * defs->runSpeed` (product order), `u32 dir -> int dir` reverted by cleanup, and the notable find: the game computes `MPoint d = mTouchStart - pos;` BEFORE `DinoFieldCharaGetScreenPos(&pos, player)` (uses the uninitialized pos — the schedule reads it before the call). Removing that order swap costs -0.05. All dead `perm` locals from the candidate were free to remove (the 10-replacement clean held 87.328 exactly).

PlayerControl still open at 87.33 (follower loop + 20 extra frame temps; the resumed permuter continues from best-6533 in ff-decomp-pc). Remaining in these overlays: DinoFieldRadar::Idle 66.75, OnStart 92.23, StateClose 96.44, Fraction4 97.81 (this+i*4), CreateCloseTask 98.24, and the 99.x slot puzzles.

---

## Session update 2026-11-08 (third pass, round 3 — PlayerControl finished)

- **DinoFieldPlayerControl::Idle 87.33 -> 88.95** (43639f80): the `--resume` round (own score 6533->5962, report +1.62). Additional shape: FindFollower loop as `for (u32 i = 0; i < 100; ++i)` with `chara = field->mCharas[(int) i]` and the chara hoisted out — the game's non-strength-reduced index form is reached through the cast, not a pragma (all three pragmas regress, see above); AngleVector's sin kept in a `const s32 perm266` local; `mTouchFrames = 0` and `tapped = TRUE` materialized through temp copies. Every `perm` local left in the candidate was removable for free (13-replacement clean held 88.947 exactly; only `const s32 perm266` remains, and removing it loses the match).

Confirmed dead this round: DinoFieldRaster::Idle (99.81, 284B) permuter round — best 75 = base 75 after 9901 compiles, no candidate; DinoUiDebugKasekiRockGetMenu::OnStart re-tested fresh (best 1639 = base 1639 after 4960 compiles) — the handoff's permuter-dead flag stands. IdleUnk2's "camera before t" declaration swap (the scale-above-easing diagnostic) is flat at 99.929. DinoUiFieldMenuNumber ctor `self` loop (the this+i*4 family): 94.12->85.22, hard regression — the family flag is genuine.

- **DinoFieldRadar::Idle 66.75 -> 66.77** (b7b8a73b): the `--resume` round (own score 9339->9330, report +0.014 — the smallest win yet, but real). The ONLY load-bearing change: the `sweep` zero-init now materializes into named temporaries in the game's slot order (n2.y, then n1.x/n1.y, then origin.x/origin.y, interleaved with the mtx decl: `s32 perm772 = perm378(=0); sweep.n2.y = perm772;` etc.). Renaming the perms to clean names (e.g. plain `sweepN2Y`) LOSES the gain down to 66.754 — the exact stack materialization is load-bearing, so the perm locals stay in the tree. Every other perm local in the candidate was removable for free (14 cleanups, held 66.768 exactly).

---

## Session update 2026-11-08 (round 5 — coordination, from the ov001/003/004/005 session)

To the other session (ov002/006/007 + game/m): I'm alive and grinding. My side of main is at 7c9683ca; nothing of mine is uncommitted. PlayerControl::Idle is at 88.95 (6968B, the biggest byte count left in my area) and I've launched a third `--resume` permuter round from best-5962 in `ff-decomp-pc3` — if it beats 5962 I'll re-score with objdiff and transplant. Everything else in my area is now confirmed permuter-dead or at 99.x.

Things for you:

1. **StateClose (96.44 on my diff scale, 96.60 on yours)** — your thread, the `(int)` cast is in. The remaining single spill slot (sp+0x14 vs the game's pool re-load of `data_ov001_0216aad4.mMinRadius`): the game keeps taking the pooled member's ADDRESS (`ldr r0,[pc]` fresh) on the max's min-wins path, ours keeps a stack copy. I re-read the diff this pass and couldn't find a shape that forces it — see al.py rows 96-109 of `_ZN26DinoFieldBattleStartEffect10StateCloseEv`. Leaving it with you.

2. **Two shared-header items are still under 90 in dino_field's context** and they're in your territory (`include/m/fx32.h`): `MFx32Rect::PushOut` (87.04, weak copy emitted into dino_field) and `operator+(const MFx32Rect&, MFx32Point)` (86.38, the game stores one operand copy per add). Handoff §6 says coordinate before touching — I'm flagging them as open if you want them; their fix would lift dino_field.cpp globally. I won't touch include/m/*.

3. **The `this + i*4` family** — re-verified this pass that the FieldMenuNumber `self = this` loop form regresses hard (94.12->85.22), so the family warning in §6 stands. Your DinoCleaningScore ctor fix (`self = this` loop-local, c9122af8) is the ONE place that idiom worked — for the record it did NOT transfer to the ov003 family.

4. **No conflicts pending on my side** — I pull --rebase before every push, and all my commits are within src/ov001, src/ov003, src/ov004, src/ov005 + docs/handoff.md.

---

## Session update 2026-11-08 (round 5.5 — post-coordination results)

- **PlayerControl::Idle round 3 REJECTED** — the `--resume` round (own score 5962->5930, "better") scores **88.685 on objdiff, worse than the committed 88.947**. The permuter-score ≠ report inversion struck a third time on this function; PlayerControl is now permuter-exhausted after 4 rounds (7067->6533->5962->5930 with reports 87.33, 88.95, 88.685). No change pushed; worktree removed. It stays at **88.95**.
- **CreateCloseTask (98.24) full dead end confirmed**: the entire diff is one r4↔r5 swap (game keeps `close`, used 5×, in r4 and `se`, used 1×, in r5; ours the reverse, rows 17-67 of the aligned diff). Declaring `close` before `se` (keeping statement order) is flat; both creation-reorders regress to 85-87 (game's asm creates se first, rows 14-19); AddTail call order is already identical. Register allocation is not declaration-driven here. Leaving at 98.24.

---

## Session update 2026-11-08 (round 6.5 — scan fix + three manual wins)

- **scan.py had a dedup bug** (the left/right side-dedup `break` truncated every unit to its FIRST sub-threshold function — I'd been reporting ~22 open functions when the real count is 162). Fixed (f9fae215), and the corrected frontier revealed substantial mid-90s targets that were never on any dead list: DinoFieldChara::Unk02162154 (95.62/1076B), IdleWander/IdleFollow/IdlePatrol (95-99), Step (97.23/716B), LoadZoomInPict (95.62), ZoomOutPict (96.49), FindRockHole (89.47), Decide (79.48) among others.
- **DinoField::FindRockHole 89.47 -> 91.24** (b1ad204b): `#pragma opt_loop_invariants off` + `near` hoisted to the loop's preheader. Unit-wide no regressions. (This was the handoff's documented local-best that was never committed — now it is; the pragma is function-scoped with a reset line.)
- **DinoFieldChara::Unk02162154 95.62 -> 98.03** (b2325b4e): 600s permuter round (own score ->325 / 3980 compiles). Winning shape: `d`/`height`/`range`/`speed`/`away`/`limit` hoisted to the function top in the game's order, `0 == focus` / `0 != DinoWithin` compare forms, and — load-bearing — `MFx32 perm745(1)`/`perm107(1)` materialized into named temps for the SetMotion/SetUnk2D8 calls (removing either drops to 98.006). DinoWithin helper perms all free. No unit regressions.
- **DinoField::Step 97.23 -> 98.58** (3a95a98e): manual find, no permuter — the rotation as direct member writes with **`e->mRot.y = angle*360/16;` BEFORE `e->mRot.x = 0; e->mRot.z = 0;`**. The SetRotation call form and x-first order both let mwcc hoist the 0-store before the mul (97.231); y-first forces the mul first, matching the game's emission order. Remaining gap: the two stores' slot order + one register swap in the mul chain.

New dead ends: DinoUiCleaningMenu::Decide — 4 named-local variants (ycol/xcol/row/row-col) all regress hard (69.7-76.9); the expression must stay inline in the MPoint ctor args. DinoFieldChara::IdleWander — 3 named-local variants regress (95.0-96.3); pure slot-layout with an extra store on our side. And the old handoff "32 bytes smaller frame" note on Unk02162154 was stale — the game's frame is BIGGER (0x50 vs 0x38), which is exactly what the extra named temps fixed.

- **DinoField::LoadZoomInPict 95.62 -> 97.96** (0c911ff4): 600s permuter round. The ENTIRE win is one identity cast: `CreateCrossFadeInTask((*pict)->GetPlaneMask(), frames, (u32) ease)` — removing just the (u32) collapses 97.961 -> 95.621. The fade/both locals must stay top-declared + assigned in the body (the split is structural). All other perm locals/casts removable for free. Sibling check: ZoomInPict 99.969, SlideInPict 99.969, FadeInPict 99.968, Step 98.578 — all unchanged from the committed baseline after the surgical hunk transplant (a whole-file candidate first contaminated the working tree — always re-verify with `git reset --hard` + rebuild before trusting a pre-commit sibling scan).

## Session update 2026-11-08 (round 6 — Radar 68.67)

- **DinoFieldRadar::Idle 66.77 -> 68.67** (2478633f): fresh permuter round from the committed state (base 9330, no --resume — the on-disk best-9339 is the pre-9330 candidate) ran 600s/-j 3 in ff-decomp-radar, scored **8862** after 2751 compiles. Adopted as a **surgical transplant**: the raw candidate mutated the shared static inline `DinoRadarToPoint` (perm temp materialization), which regressed **SetPlayer 100 -> 90.5** — reverted just that helper, kept Idle + the private helpers (Normal: cos hoisted to `const u32 perm765`; Swept/Between: BOOL decl splits, TRUE-through-temp, `MFx32(0 != 0)`, brace blocks; sweep zero-init slot order in Idle). Final: Idle 68.666, SetPlayer 100.000, all 33 scored unit symbols at baseline highs (no regressions). Cleanup scoped: perms here are the load-bearing slot-materialization class (as in b7b8a73b), no removable-for-free candidates found by the single-assignment scan.
- Re-verified the stale-state trap at setup: ff-decomp-radar answered the "was dirty" question — it carried an uncommitted post-9330 experiment (perm locals in radar helpers) that is NOT in the committed tree; preserved to ~/.hermes/cache/scratch/radar-preserve/ before resetting. Also the worktree needed its gitignored extract/ back (symlinked to the main repo's), which try_match requires for symbol addresses.
- PlayerControl stays at 88.95 (round 3 rejected by objdiff, see round 5.5 update); the radar worktree is idle and clean at origin/main if anyone wants the next radar resume (base would now be the committed 8862-equivalent state).
