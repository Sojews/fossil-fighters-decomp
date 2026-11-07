# Matching playbook

How to turn a near-miss into a match for this game: mwccarm 2.0 (sp1p5 configured; every 2.0 build gives the same
code here), `-O4,p -proc arm946e -thumb`, most game TUs built with `-ipa file`, MSL C++ templates. Organized by the
symptom you see in the diff. Each entry is a source-level change that has matched or measurably moved real functions;
the function named after it is where it worked (grep `src/` for it to see the code in place).

Contents: [Workflow](#workflow) · [Reading a diff](#reading-a-diff) · [Register swap](#register-swap) ·
[Stack and spill slots](#stack-and-spill-slots) · [Frame size](#frame-size) · [Branch polarity and block layout](#branch-polarity-and-block-layout) ·
[Extra or missing instructions](#extra-or-missing-instructions) · [Evaluation order](#evaluation-order-and-scheduling) ·
[Literal pools and data anchors](#literal-pools-and-data-anchors) · [Static init (`__sinit`)](#static-init-__sinit) ·
[Calls, docking, template placement](#calls-docking-and-template-placement) · [Inline or out of line](#inline-or-out-of-line) ·
[Switches](#switches-and-jump-tables) · [MSL templates](#msl-templates) · [Types and layouts](#types-signatures-and-layouts) ·
[Low-% functions](#low--functions) · [Dead ends](#dead-ends) · [Unattended sweep](#unattended-sweep)

---

## Workflow

**Build and score.**

```sh
python3 tools/configure.py usa && ninja                     # incremental (tools/fix_dep.py fixes the .d paths)
ninja objdiff.json && tools/bin/objdiff-cli report generate -o build/report.json   # ~0.3 s
```

- **`build/report.json` is the truth** for percentages and the only thing that moves the metric. `objdiff-cli diff`
  and `tools/try_match.py` disagree with it in both directions:
  - The report forgives call-target *names* (a `bl` to the wrong overload or to our own weak template copy scores
    100). `try_match` shows our `bl` vs the game's `blx` to ARM code as a diff; the report doesn't.
  - The report docks a byte-identical function by ~0.005% when its `bl` targets the symbol right after it (see
    [docking](#calls-docking-and-template-placement)).
  - Relocation targets are compared by address for placeholder `data_XXXXXXXX` names, by name for real symbols.
- **Fast inner loop:** edit, `ninja build/usa/<unit>.o`, regenerate the report, read the one function's
  `fuzzy_match_percent`. Use `.venv/bin/python tools/try_match.py src/foo.cpp <mangled>` (needs capstone) for a
  side-by-side disassembly; it filters by the mangled name.
- New units need `ninja` and then `ninja objdiff.json` before the report sees them.

**Gate before every commit.** Keep a report from before the change, then after the change check:

1. `ninja` succeeds (the ROM check passes);
2. no function that was 100% in the old report is below 100% now;
3. `python3 tools/callcheck.py | sort` has no line that wasn't there before (calls whose target differs from the
   game's; the report ignores these).

`tools/overnight_sweep.py` implements exactly this gate (plus "no other function loses fuzzy %"). Commit message
style: `Foo::Bar 97.10->100: <the idea that did it>` or `Match Foo::Bar (<idea>)`. Commit wins one by one, with
`git add <files>` (never `-A`).

**Permuter.**

```sh
.venv/bin/python tools/permute.py src/ov000/foo.cpp <mangled> "void Foo::Bar(" -t 600 -j 6 [--resume] [--no-helpers]
.venv/bin/python tools/permute_batch.py --min 95 --max-size 1500 -t 240 -j 6 --units src/m src/game [--list]
.venv/bin/python tools/permute_min.py src/ov000/foo.cpp <mangled> build/permute/<mangled>/best-0.cpp
```

- Outputs: `build/permute/<mangled>/best-<score>.cpp` (whole file); `best-0` is a match and stops the run. Small
  functions finish in ~100 s; cap long runs at ~10 min. `permute_batch` logs to `build/permute/batch.log`;
  `--improved LOG` reruns the ones that improved, `--retry LOG` the ones that failed to run.
- **The permuter score does not track objdiff %.** A 293 -> 244 drop can be worth 0.00%; a better score can be a
  lower %. Gate every candidate with the report.
- **Keep the no-op-looking casts** it adds (`(u32) w1`, `(int) w0`, `(u32) i`): they are often the whole fix
  (GetMorphVertices, DinoCommonWindowMenu::OnStart). Remove them one at a time, never all at once.
- **Check semantics.** Outputs delete `x < 0` branches via `u32`, reorder calls or stores into a struct, read
  uninitialized locals, add `*++dst`. Use those as hints, then write a correct version by hand. Compare the game's
  store/call order before keeping any reorder.
- Old outputs are worth mining: revert hunks back to the original while the function's % holds
  (`permute_min.py`, or `overnight_sweep.py --mine`), then judge the remaining edits by hand.
- **Templates:** `permute_batch` skips template instances. Paste the member as an explicit specialization in the
  .cpp (`template <> RET Class<Args>::Method(...) {...}` above the `template class` line), check it scores the same,
  permute that, then port the edit back into the header and recheck every instance (MOamCanvas::AddPiece x2).
- At >= 99.9% it is mostly saturated: rerunning without new mutation kinds rarely pays.

**Search over source edits by hand or script.** Single edits miss combinations (MDisplay::SetBgMode needed three
coordinated edits; the first alone scored lower). Useful edit kinds, in order of payoff: hoist a call subexpression
into a local (`__typeof__(e) t = e;` works in mwcc); `x op= y` <-> `x = x op y`; `int`/`u32`/`s32` flips (note `s32`
is `long`, different from `int`: PopNumber ctor); adjacent statement swaps; swap names in `T a, b;`; commutative
operand swaps (precedence-safe); split `T a = e;` into `T a; ... a = e;`.

**Per-function pragma sweep.** Wrap one function in each of these and keep what moves it:

```cpp
#pragma opt_common_subs off          // ... function ... then #pragma opt_common_subs reset
#pragma opt_strength_reduction off
#pragma opt_loop_invariants off
#pragma opt_dead_assignments off
#pragma optimization_level 3        // put it BEFORE other opt_* pragmas: it resets them
#pragma always_inline on            // DinoCleaningSampleMenu::OnStart 51 -> 94
#pragma inline_max_size(2000)       // big builders whose inline ctors were emitted as calls
```

A pragma around a *template member definition* does nothing (templates compile at instantiation). Instantiate just
that member inside the pragma, before the `template class` line, with its helpers explicitly instantiated first
outside the region (`vector<char>::reallocate_copy_trivial`, deque dtors). A function inside a file-wide
`opt_common_subs off` region may need CSE back on: move it out (SelectAllMenu::OnStart).

**ipa.** `#pragma ipa file` is file-wide and must come before any declaration. Under ipa: unreferenced statics are
dropped; same-size statics come out in reverse declaration order; functions defined later in the file can be inlined;
nested inlines expand (non-ipa mwcc expands only one inline level); deferred inline/template functions are emitted in
roughly reverse instantiation order. To convert a TU: add the pragma, then
(1) wrap the definitions of callees the game calls out of line in `#pragma auto_inline off` / `reset`;
(2) route `push_back`s through a `const T &` helper if the byte temps swap (see [MSL](#msl-templates));
(3) read file statics the game loads through its pool symbol if ipa folds them;
(4) fix [docking](#calls-docking-and-template-placement). Test with a full report: ipa often gains some functions and
loses others in the same TU.

---

## Reading a diff

Count-based comparisons of game vs ours per function find leads that the fuzzy % hides:

| difference | usually means |
|---|---|
| `asr` vs `lsr`, `ldrsb` vs `ldrb`, `blt` vs `blo`, `_s32_div_f` vs `_u32_div_f` | wrong signedness of a member/local (UseCreatureSelect::Select 91 -> 99.3 via `int` members) |
| `ldrb`/`ldrh`/`ldr` pair counts | wrong member width, or a struct copied member-wise vs as a block |
| different `sub sp, #N` | missing/extra local, temp, or wrong local array size ([Frame size](#frame-size)) |
| different set of vtables or `bl` targets | a real source bug (wrong task-set class, invented call) |
| different `new` sizes | wrong class size (trailing member missing) |
| incoming stack param copied more/less | an inline takes it by `const T &` (DinoCommonLabel ctor via `DinoLabelText(const u32 &id)`) |
| `mov r0,#1; b; mov r0,#0; cmp r0,#0` | a bool inline evaluated once and reused ([MSL](#msl-templates)) |

Read the actual instruction diff (what the report scores), not a masked one: branch-target masking once hid a
`== 0 ||` that should have been `!= 0 &&`. Check class bases against the game's type info: a wrong base class
shows up as wrong vtable stores in the owner's ctor (DinoBattleViewBackdrop derives from MRasterIO, not
MRasterPal; `tools/rtti_names.py` and docs/classes.md).

---

## Register swap

Same instructions, two registers exchanged.

- **Split or share loop counters.** One `u32 i` declared at the top and shared by several loops fixed a three-loop
  swap (ResetAnime); elsewhere each loop needs its own `for (u32 i ...)` (CreatureInfoMenu::OnDecide,
  CameraWipe::Idle: per-loop `int y` instead of one shared by switch cases). A separate variable declared just before
  one loop fixed a 3-register rotation (DinoCleaningScore::Idle). Try both.
- **Hoist a chained call into a local:** `MDisplay *display = gLcd->GetBottomDisplay(); MBright *bright =
  display->mBright;` instead of `gLcd->GetBottomDisplay()->mBright` (keyboard Finish/OnBackDecided,
  ReviveStatusMenu::Leave; the old "fade constants swap registers" dead end).
- **An existing inline accessor instead of spelled-out math** changes register choice: `chr->GetTileAddr(tile)`
  for `chr->addr + tile * chr->unitSize` (M1BitFont/M4BitFont::Draw).
- **Declaration order of locals** and `T a; ...; a = ...;` split declarations (MPalLoadTask::Transfer src/size).
- **Increment placement:** move `data++` out of a call argument (MPalAnime::AddBlend); `*dst = f(); dst++;` instead
  of `*dst++ = f();`, counter as `while (n != 0) { ...; n--; dst++; }` (AddBlend 85 -> 99.5).
- **Index vs walking pointer:** if the game's pointer is the compiler's strength-reduced induction variable, write
  `text[i]` / `from[count] = x; count++` instead of a hand-written walking pointer (MBg::SetMode,
  func_ov000_0217df4c). Conversely, post-increment pointer statements `*out++ = (*prev++ * w0 + *next++ * w1) / d;`
  took GetMorphVertices 77.9 -> 98.1.
- **A register swap inside an inlined block = a further inline split** (DinoReviveLetGo).
- **Operand order of a 3-register add:** game `add rD, rA, rB` vs ours 2-reg form = swap the source operands
  (`scales[i + head]`, `GetText(k + base - 1)`). `mul Rd, Rm` puts the *second* source operand in Rd.
- Inside an `opt_common_subs off` function, a pointer the game reuses across an if/else-if chain is one named local
  over the whole chain (`MInput *input = mInput;`, RvInformation3DModel::Idle).
- A local flag as `bool` vs `BOOL`/`u8` changes when it is zeroed (shop choices' masks: `u8`).

---

## Stack and spill slots

Rules measured on toy files and confirmed on real functions:

- **Named locals** take the top slots in **declaration order** (first declared = highest), independent of scope.
- **Temporaries** go below all named locals, in **lexical (source) order**, first created highest.
- **Temps created inside inlined bodies** (inline keyword, auto-inlined, in-class ctor, ctor via `new`) **ascend**,
  sit below the caller's temps, deeper inline levels below shallower; a second instance of the same inline sits below
  the first. A macro gives the caller's (descending) order.
- **Named locals inside an inline are laid out ascending.** To get them descending below the caller's locals, pass
  the values as the caller's expression temporaries through `const &` params:
  `PickExchangeId(gDinoResource->GetExcavateDefs(), gDinoResource->GetExcavateDefs())` (MakeExchangeEntry).
- Local arrays in one scope are sorted by size (largest highest); a block-scoped array sits below outer ones.
- **Spills:** among values live across calls, ours spills the one with the **fewest uses first**, into the lowest
  slot; spilled values never share a slot. Declaration order doesn't decide spill order.

Levers:

| symptom | fix |
|---|---|
| game slot *above* a named local | name it and declare it earlier: `MPoint dest(368, y);` instead of a temp arg; `MPoint from;` declared before `to` (AbilityMenu::OnDecide) |
| a temp must sit *lower* | build it inside a non-member `static inline` helper (`DinoBgOffsetY(bg)`; ImageView copy ctor/CreateScreen; Org CreateRevertTasks `CreateFlyInTask(DinoOrgSlotPos(index % 15))`) |
| game spills a strength-reduced `i*size` offset (`ldr/adds #8/str [sp]`) but ours keeps it in a register and spills a plain local | write the offset as a named counter (`u32 off = 0; ... ((Entry *)((u8 *)&v[0] + off))->x; off += sizeof(Entry);`), declared inside an `if (i < size) do {} while` guard so its init lands after the guard; fold a `+ 1` into a spilled local (`count = size() + 1`) to flip its slot (MSimpleMenu::Layout) |
| two spill slots swapped | change relative use counts: re-read the global/member at one use instead of going through the local (AbilityMenu::Close) |
| hoisted `&mParallel` must be in the LOWEST slot | non-member `static inline MTaskSet *P(Menu *m) { return &m->mParallel; }` called inside the loop (`P(this)->AddTail(x)`); param must be the concrete class (RootSelect, Medal, Museum OnStart) |
| loop-top cached member address | `MTaskSet *parallel = SomeNonMemberInline(this);` as the loop body's first statement (DinoFieldDance::Start, KasekiButton::Setup) |
| two calls on the same member set | `MTaskSet *parallel = &mParallel; parallel->Remove(a); parallel->Remove(b);` (ViewPlayer::OnMessage) |
| several temps of one `AddTail` run in the wrong slots | write ONE multi-arg expression `set->AddTail(a(), b(), exit)` (args evaluate right to left) instead of locals |
| one slot reused across statements for `std::string("")` | it's a **default argument** (`const std::string &t = ""`, call omits it); explicit temps get one slot each |
| std::min/max temporaries swapped | swap the declaration order of the inline's locals (`u32 sum, max;` vs `u32 max, sum;`) |
| `int2type`/tag byte temps swapped | see [MSL](#msl-templates) byte-temp rule |
| `new X(by-value MPoint args)` temp order | `#pragma ipa file` + `static inline X *MNewX(...) { return new X(...); }` (six m/ transfer factories; no effect without ipa) |
| a value the game keeps on the stack | `T *volatile self = this;`; for a byte temp, a `volatile` by-value param (`__msl_assign(T &d, volatile T s, ...)`) |
| entry copy of a stack-passed param to its home slot | read it through a volatile into a local (ParseScript) |
| `a = b = -1` | reversing declarations flips two slots (ParseScript) |
| two spilled locals: the game stores A first but gives B (stored second) the higher slot | ours gives the higher slot to whichever is defined first, so give B a dead first initialization above A and assign it again after A: `B f(x); A last2(y); f = B(x);` (MSL __insertion_sort_move x6) |
| dead counter the game stores | survived DCE; `#pragma optimization_level 3` or `opt_dead_assignments off` keeps dead struct-temp stores (AddLink) |
| a loop's helper temps vs caller temps | write the helper's loop in place instead of in an inline (State02161f9c, CreateAttackTask) |
| list-iterator-heavy function keeps several iterator copies on the stack | `#pragma optimization_level 3` (HasBase 55 -> 91); by-value iterator inline `IsFound(it)` (SwitchThreadDisplay) |

Declaring all of a function's counters/flags at the top also stopped mwcc cross-jumping two identical branches
(DinoCleaningReport::State02155b28).

---

## Frame size

- Diff the *sets* of `sp` offsets used (ours vs game): a constant shift above some slot = one extra/missing temp or
  spill there.
- **Local buffer too big:** `char text[20]` -> `char text[8]` when the game's temps sit inside our buffer
  (PopNumber ctor). The frame-size difference is the sum of size differences.
- **MMutexLock** is register-promoted (no frame slot) only when no call sits between lock and unlock; with a call
  inside, it stays in memory. archive.cpp also needed `#pragma ipa file` for the promotion (MDumpArchives).
- A missing by-value temp shows as a smaller frame: `GetFrame().Moved(offset)` (SelectAllMenu::OnStart),
  `MRect::Moved(dx, dy)` returning by value (DinAttackItemHelp::GetRect).
- MIrqLock written out (`u16 prev = REG_IME; REG_IME = 0; ... REG_IME = prev;`) keeps the flag in a register
  (SwitchThreadDisplay 77 -> 87).

---

## Branch polarity and block layout

- **0/1 value polarity:** MWCC canonicalizes int-armed `x == 0 ? 0 : 1` (and if/else, `!x`) to true-first
  `cmp; beq Lzero; mov 1; b; Lzero: mov 0`. When the game has the other order, use **enum-typed arms**:
  `x == 0 ? ENUM_A : ENUM_B` or `focusYes ? E_YES : E_NO` (func_ov000_0216dec4, View3DDamage::Idle,
  TurnEndDialog ctor, DinoMpGame::StateStart). Only bool-valued arms are canonicalized.
- **`return` instead of `break`** at the end of a switch case when the game jumps straight to the epilogue
  (ViewPlayer::OnMessage 98.9 -> 99.98).
- **`if (x == 0) return;` vs an if-block** decides whether the early exit chains to a nearby epilogue (ShowCreature
  needed the return; index_bitmap LoadTransfer the block). One nested `if (a && b) { ...; return x; } return prev;`
  instead of two early returns (GetMorphVertices).
- **Loop entry tested twice** (`cmp; ble X; bgt body; X: b end`, two identical `ble`s) =
  `int i = 0; if (n > 0) { for (; i < n; i++) ... }` (AbilityMenu::Close, func_ov000_0217df4c).
- **`cmp #4; blo; beq; cmp #5`** = `if (mIndex >= 4) switch (mIndex) { case 4: ... case 5: ... }`.
- **Switch case bodies are laid out in source order** (the dispatch doesn't depend on it): reorder `case`s to match
  the game's block order (BinFont `case 3: case 2: case 1:`).
- **Adding a function can flip a later function's branch layout** in the same TU (`bgt`<->`ble`); put new
  definitions at the end of a partial TU.
- `x != 0` (not `x ? 1 : 0`) for a BOOL bitfield arg keeps the game's evaluation order.
- Unsigned guard folding: an inline helper with a `u32 n` param folds `0 < n` to `beq`; making `u32 n = count;` the
  helper's first local keeps `bls`/`blo` (MAnime::Execute).

---

## Extra or missing instructions

**Game recomputes what ours CSEs / hoists:**
- Give the two uses different types: `Next((int)n * 2 + 1)` then `Next(n * 2 + 1)` (TalkWindow::StateShow 83 -> 100);
  `(u32)(index % 4) == 3` on one axis (DinoUiCleaningMenu::Decide). Two reads of one member through differently
  typed lvalues aren't CSE'd.
- A **`const` inline accessor** makes mwcc reload the member pointer instead of sharing it (ShowCell).
- `*(volatile const T *)&x` for a load the game repeats in a loop (DinoCommonLive::CheckCondition).
- A bool array index recomputed per use, each into its own temp = `cond ? 1 : 0` (plain `!cond` gets hoisted).
- Per-function `opt_common_subs off` / `opt_loop_invariants off`.

**Game keeps `lsl i,#2` / re-multiplies each iteration where ours strength-reduces:**
- `#pragma opt_strength_reduction off` around the function (DinoUiShopBuyMenu::Restock).
- A u32-index accessor: `static inline u32 F(const u32 *t, u32 i) { return t[i]; }` (CountAttacks x2), or existing
  accessor inlines (`def->GetAttackLevel(i)`). Signed `for (int i ...)` over `v[i]` stops SR where `u32` gets it.

**Ours keeps a value the game reloads (or the reverse):**
- Inline accessor with `GetTopLeft()` so fields reload (MOamCanvas::SetPixel); read `piece.rect` fields directly.
- Re-index `mUnit->GetEntries()[i]` per use instead of an entry pointer (MOam Alloc).
- `if (mScreens[i] != NULL) mScreens[i]->SetBg(i)` after the store (no reload).

**Dead copies / extra stores the game has:**
- A by-value MFx32 param stores a dead copy: declare the param `MFx32` and pass `MFx32::FromRaw(0)`.
- `MPoint(0, 0) - p` instead of `MPoint(-p.x, -p.y)` (keeps a component in a register across a call;
  SetOffsetAll); `mOffset(MPoint(0, 0) - dst)`; `MRect(dst, dst + size)` (extra MPoint temps).
- An inline that modifies its by-value MPoint param in place (`cell.x -= o.x;`) = two stores to one temp slot.
- Member read twice around a post-increment then stored:
  `int next = x + 1; int prev = *(volatile int *)&x; *(volatile int *)&x = next;` (both volatiles; HammerEffect).
- `u64 d = denom; REG = d;` spills the zero high word like the game.

**Truncation/width:** `(int)(u16)x` into a u16 = double `lsl/lsr #16`; obj.h `y = (u8)pos.y` = double `lsl/lsr #24`;
MFx32PointAngle adds the game's u16 truncation. `s32` (long) weights vs `int` differ.

**Constants kept as runtime values:** the variable is assigned twice (e.g. again in one switch case), which blocks
constant propagation.

---

## Evaluation order and scheduling

- **Inline functions/ctors evaluate args right to left**; class-type (by-value MFx32/MPoint) args are evaluated
  first, each with a temp copy. A parameter the game loads early and copies is a class type (fix the signature and
  symbols.txt mangling: CharaMoveVector, StartWander).
- **`AddTail(a, b, c)` evaluates c first**: `mSerial.AddTail(new MTimer(10), MTaskExitTransfer::Create(this))`
  creates exit first. Chains: `(new MParallelTaskSet(TRUE))->Add(a)->Add(b)` (receiver last).
- **CW calls `operator new` before evaluating ctor args**, so field loads come after the call: write one
  `new T(args)` expression.
- An argument written as an **inline accessor call** (`GetOam()`) is evaluated into a temp before the other args;
  a plain field the game loads before a sibling call = read it through an inline accessor.
- **Member call on a temporary copies its argument before building the temporary** (`MRect::MovedTo`,
  YesNoMenu::OnStart).
- mwcc substitutes a pure inline argument lazily where the param is first read: copy it (`int rows = h;`) inside the
  helper to force early evaluation (73 -> 85).
- **MFx32 `+=` vs `x = x + y`** decides which arg slot the running total takes and the order: game total in the lower
  slot = `+=` (all five DinoBattleCpuController::Score*). Accumulations may want compound operators throughout the
  function (`result += ...; result -= ...; percent /= ...`: CalcDamage). Same for `x *= y` vs `x = x * y`
  (KasekiRock::StateDrop/Hit, DinoFieldRadar ctor).
- Postfix MFx32 `operator++` (`old = a; a += 1; return old`): `if (mFrame++ >= mDuration)` reads the right operand
  first.
- Expression evaluation order: `(palette << 12) | tile` (shifted operand first; MText x3); RGB555 as
  `B | (G | (R | 0x8000))` (Backdrop, TalkWindow ctor). `std::max<int>(labs(a), b)` with only b named.
- A named first point arg (`MPoint from(256, 192)`) flips which constant is derived from a register
  (TalkWindow::Close 90 -> 99.9).
- `it->first == call()` -> put the call result in a local first (DefenderMenu::OnStart).
- **`extern "C" const` on pooled data** lets its load schedule above member stores (B3DStatusViewTask::StateNext).
- One call after an if/else in the game = a ternary argument: `mParallel.AddTail(c ? A() : new MTimer(1))`.
- A member store placed before a vector member's inline init = that member is in the ctor init list.
- `u32 body; if ((body = state.field) >= Get()->count)` = field loaded after the call and kept in a register.

---

## Literal pools and data anchors

- **Constant-index reads of a pooled table go through the preceding object's literal (`anchor+off`)**, indexed reads
  through the table's own symbol. Write the constant-index read as `((const T *)anchor)[n]` (MaskBuyMenu RefreshPage2);
  a grid read through a struct-pointer cast (`#define GRID ((const Grid *)data_...)` with padding) instead of
  `*(const MPoint *)(pool + off)` helpers.
- `ldr [base,#off]` vs our folded `sym+off` literal: read via `((const int *)&sym)[n]`.
- A literal `_ZTI<Class>+N` read as data = the TU's .data pool anchor (typeinfo comes first under ipa):
  `*(const T *)((const u8 *)&typeid(Class) + N)`.
- **std::min/max against a pooled limit binds the constant's address:** `std::min((u32)v, *(const u32 *)&sLimits[n])`
  (SetSystemVar). A literal bound to the const-ref param is pooled; the game's `mov r0,#0xFE; str [sp]` stack temp =
  the literal came from an inline function's return (`std::max(tbl.winFrames, DinoWonLeast())`, StateWin). A pool of
  identical words picked by address = `std::min((u32)0xFF, x)` with compiler literal temps. MSL min(a, b) returns b
  only if b < a strictly: the game's `cmp x,lim; bhs -> lim` is `std::min(lim, x)`; swap args when the compare is
  reversed (OnPostLoop 86 -> 99.97).
- Pooled std::min/max operands addressed via dsd's split symbol: a TU-local
  `static inline const T &Min(const T &value, const T &named, const T &b) { return b < value ? b : named; }` with the
  split symbol declared extern (drill, score).
- **Pooled-constant offset** (`ldr r0,[anchor,#K]` vs `#0` for a template's `const T &` literal): spell the
  instance out as an explicit specialization reading the value from the TU's anchor at +K (get_temporary_buffer;
  `insert_unique` with pooled bools: exchange TU true +1/false +2, text_codec the reverse; DinoMacAddress set).
- Literal-temp pool offsets: dead `std::max(0, 0);` statements before/after a clamp shift our literals to the game's
  offsets (MMotion::GetCellRect).
- **Non-ipa file data offset:** an unused `static u32 pad[N]` placeholder ahead of tables reproduces anchor+offset
  (ipa files drop unreferenced statics, so read guards through the static itself there).
- `const` vs non-`const` extern on main-module constants changes scheduling (hoistable without volatile).
- A macro's operand evaluation order decides which offset pairs with which literal (LERP evaluates `to` first).
- The report already ignores reloc *name* differences at the same address; don't edit relocs for those. Edit
  `relocs.txt` to `to:<base> add:<off>` only when the delinker's `data_X` lands inside a typeinfo/vtable we emit.

---

## Static init (`__sinit`)

Macros in `include/game/dino_static_init.h`.

- `DINO_POOL_WORD` is volatile (fixed store order); `DINO_POOL_NV` is not (scheduler may reorder). Use NV for the
  first N stores in offset order when the game interleaves them (wireless_cancel sinit).
- **Pool offsets >= 0x80 go through a second anchor** (the data symbol at pool+0x80, loaded first):
  `DINO_COMMON_LAYOUT_INIT2` / `DINO_POOL_AT` (stop_confirm 80 -> 94.6). Conversely, writing a later 0x80 block
  through the FIRST anchor with a bigger offset (`DINO_POOL_WORD(A, 0xe4)`, not `(A80, 0x64)`) fixed "constant mov
  scheduled one store later" in 7 sinits. The literal-pool order shows which anchor the game used.
- u16 stores: `((volatile u16 *)sym)[off / 2] = 0x801f` (`DINO_POOL_WORD_16` is s16 and sign-extends).
- Point pairs where only the last store differs: `volatile s32 *p = (volatile s32 *)&data_X; p[0] = x; p[1] = y;`.
- A sinit whose declaration order conflicts with the readers' layout: keep the statics as POD/no-op ctors and rebuild
  `__sinit` at EOF as a static struct ctor writing through the game's anchors (DinoResource 17 -> 100). Adding the
  `dino_static_init.h` include alone shifted 24 functions in one TU: define the macro locally then.
- `std::string arr[3] = {paths[5], paths[9], paths[6]};` constructs and registers elements one by one, matching three
  `__register_global_object` calls (disp_blower).
- One stack copy too many for a rect built from a just-stored size: pass the pool point itself
  (`MRect(MPoint(0, 0), DINO_POOL_NV(MPoint, sym, off))`), not a local copy; a by-value arg costs temp + copy, a
  pool lvalue only the copy. A named offset local computed before the rect's store fixes the slot order and delays
  the rect's stores (dino_main_layout 94.6 -> 100).
- **Common-layout sinit in an `#pragma ipa file` TU** (dead stack temps in the mirrored slot order): move the body into
  a `static inline void Init()` the struct ctor calls and write the rects as `MRect(MPoint(0, 0), pool size)` (not
  `DinoSizeRect`): the extra inline level gives the game's slots. If the game hoists only the rect's zero half above
  the size stores, store the rect field-wise from a local `MRect r(...)`: top-left through the anchor at pool+0x80,
  size half through the pool's own symbol (revive field suspend sinit 79 -> 100).
- A static-init struct object takes 1 byte of .bss: place it after the TU's other .bss statics.
- **.bss layout:** our pool puts 4-byte statics first and orders same-size objects by declaration (ours = declaration
  order rotated left by one, the game rotated right; dtor records after each object in ours, before in the game).
  `tools/static_order.py` finds a declaration order for a wanted layout when one exists.
- Gap sinits (dsd left them in `_gap@` ranges) can be matched as a standalone `.init`-only `complete` unit; a complete
  unit must also claim its `.ctor` slot. Round the `.init` end up to 4.

---

## Calls, docking and template placement

- **Docking:** the report scores a byte-identical caller ~0.005% short when its first `bl` targets the symbol emitted
  right after it in *our* object (bl target - 4 falls inside the caller). Fixes, in order:
  - If the game's unit doesn't contain that callee at all (linker-deduplicated weak copy), keep it out with a per-TU
    guard macro: `MSL_STRING_COMPARE_ELSEWHERE` (libs/msl/include/string, used in m/file.cpp),
    `M_RESOURCE_CTOR_ELSEWHERE` (include/m/resource.h), `MSL_TREE_DTOR_ELSEWHERE` (m/motion.cpp),
    `MSL_ROTATE_ELSEWHERE` (museum).
  - If the game has the callee later in the unit: an explicit instantiation of the caller inside `auto_inline off`,
    placed after the file's last function, emits the caller there and defers its callees to EOF (m/pal
    `sub_push_back`). Works only in ipa TUs.
  - An inline member defined before its caller vs after: a `bl` into a tiny stub right after a function = an inline
    member defined after its caller (mwcc keeps the call and emits the copy). OnBackDecided needed RefreshName
    defined before it.
  - When the game's `bl` goes to its next symbol, ours scores only if we also emit that callee right after (explicit
    instantiations in game order, ipa only).
- **Report vs `objdiff-cli diff` gap:** report > CLI = the report already forgives next-function rows; report < CLI =
  a call into a template copy the game keeps in another unit -> **declare (don't define) that specialization** so ours
  calls the external copy (StateDig).
- Re-test old "game-side docking" notes: an explicit `template MTask *DinoCreatePostAndWaitTask(...)` above the caller
  matched DinoBattleModelCreature::OnMessage.
- Call-target mismatches (report-invisible) are listed by `tools/callcheck.py`; a common cause is a by-value vs
  by-reference overload or the C1/C2/D1/D2 variant.
- A missing thunk = its class's key-function dtor sits at the very end of the previous unit's range: move the dtor
  into the next unit's .cpp and move the delinks boundary.
- Symbol size: if the game has non-zero bytes between a symbol's end and the next symbol, the size in symbols.txt is
  short (DinoReplaceTag 0x66 -> 0x68). A mis-mangled symbols.txt name (wrong length prefix) makes a function
  unmatchable: the report shows it as None.

---

## Inline or out of line

- Game calls it out of line in an ipa TU -> wrap its *definition* in `#pragma auto_inline off` / `reset` (ipa decides
  by the callee's pragma). Game inlines it -> mark the definition `inline`, or `#pragma always_inline on`.
- Big builders whose inline ctors come out as calls -> `#pragma inline_max_size(2000)` around them; if that also
  inlines a non-inline template (MCreateCallTask), declare an explicit specialization before the caller and define it
  after.
- A template the game calls but ipa inlines -> explicit specialization declared before the caller, defined after it
  under `auto_inline off` (keyboard TU).
- Non-ipa mwcc expands only one inline level; nested inlines need `#pragma ipa file`.
- `MFx32Const(0.3)` (include/m/fx32.h) builds an MFx32 constant in place (DebugLog ctor); a plain `MFx32(double)`
  ctor isn't possible (ambiguous with `MFx32(u32)`).
- Don't move MSerializable/MMessage/MTask dtors out of line (derived dtors need them inline).
- Function-local statics at known addresses: emulate with extern structs and call the ctor by its linker name
  (`extern "C" void _ZN6MPointC1Eii(MPoint *, int, int);`); placement new adds a null check.

---

## Switches and jump tables

- Our mwcc: a case group gets a jump table when it has >= 4 cases and span <= 3*count - 3 (clusters join when span
  <= 3*count - 2), then a balanced tree with root at index (n-1)//2. `#pragma switch_tables` is ignored.
- An empty `case X: break;` the original had (invisible in the asm) changes the grouping
  (DinoBattleViewCreature::OnMessage got the game's tree with `case 0x1002E: break;`).
- **`||` chain instead of a switch** gives compare-and-branch where a switch would make a table
  (`if (i == 1 || i == 2 || i == 3 || i == 6)`, func_ov007_02182020 82.6 -> 99.96).
- The jump table's entry order reveals case numbering: code is laid out in source order, the table maps value->block.
- Padding a switch with dummy cases can raise the % without being the game's shape: don't bank it.

---

## MSL templates

Headers in `libs/msl_cpp/include`. Any header change needs the gate over the whole ROM.

- **push_back byte temps (unk/tag) swapped:** push through
  `static inline void Push(std::vector<T> &v, const T &x) { v.push_back(x); }` (DinoCleaningImageView::AddImage).
  For `vector<T*>` the helper must be non-template: `static inline void MPush(std::vector<X*> &v, X *const &x)`
  (vector<T*> shares vector<void*>). A cast at the call site (`(void *const &)i`) swaps them; do the cast inside a
  helper taking `const u32 &` (DinoPushIndex). `resize` likewise via a helper.
- **Byte-temp rule:** under ipa the first-created byte temp gets the higher byte of the word, without ipa the lower:
  push_back matches only without ipa, append_realloc's tags only with ipa (~22 append_realloc near-misses at 93-97%
  = the TU lacks `#pragma ipa file`).
- `std::__vec_push_tag<T>`: push_back's slow path makes its tag directly (AddFile, DinoBattleView3D ctor).
- `__vec_move_tag` = `is_scalar`: the game memcpy+memsets scalars, pointers, strings and vectors on realloc. A class
  element copied member-wise needs a user copy ctor plus `Metrowerks::is_trivial` and `has_trivial_copy_ctor`
  specializations (MPalAnime::Frame, StatusEffectEntry).
- A 78% append_realloc that ignores its value arg and zero-inits a temp = the `(n)` overload misnamed.
- **tree/list insert guard:** `__node_hold` takes `&p_` in its dtor and constructs via `&hold.p_->data_`; route the
  pointer through one more by-value inline hop to order the two copies. In non-ipa TUs a holder's member is reloaded
  after stores; if the game forwards it, the TU was ipa (ui_exchange tree insert 94.9 -> 100).
- Tree erase: `MMenuLinks::iterator e(it.node_); mLinks.erase(e);` (named copy; MMenu::RemoveLink/RemoveItem);
  `iterator(it.node_)` reuses a tested node (StateRun).
- A bool inline evaluated once as 0/1 and reused = a local taken first:
  `const wchar_t *e = text.end().p_; set.insert(text.begin(), const_iterator(e));` (MSetUnkTextChars).
- reverse_iterator / string const_iterator `!=` as `bool r = !(lhs.base() == rhs.base()); return r;` materializes
  the 1/0 compare.
- `map.find(k) != map.end()` tested in an `if` = a count-like inline
  `static inline u32 Has(Map &m, const u32 &k) { Map::iterator it = m.find(k); return it != m.end(); }`.
- Map loop: `for (it = m.begin(), end = m.end(); it != end; ++it)` + `u32 key = it->first;` (DefenderMenu).
- `*(v.end() - 2)` in the game = `*(v.begin() + (u32)v.size() - 2)` ("copies the size before scaling").
- vector insert: in-place path first: `if ((n <= c && size_ <= c - n) || expand_by(...)) { in-place } else { realloc }`.
- vector range ctor = `vector(It, It, const Allocator &a = Allocator())` + `choose_init(first, last, int2type<false>())`.
- `__insertion_sort_move` advances `first` itself (guard `!(first == last)`); `__inplace_merge` cut iterators declared
  at the top, distances via by-value `__distance`. A shape shared by many template instances points at a header line.
- deque iterator `operator-` as `tmp += -n; return tmp;` gives the separate return copy.
- `__temp_buffer_iterator::operator+` as `return __temp_buffer_iterator(*this) += n;` (no named tmp): fixed the
  comp/buf_ register swap in every light-comparator `__stable_sort_move` (+12, incl. DinoBackupEntry). An iterator
  operator's temp form changes the caller's regalloc even when the emitted copies look identical.
- `~list() { this->erase(this->begin(), this->end()); }` keeps iterator temps' slots under ipa.

---

## Types, signatures and layouts

- If a note blames "dead temporaries in a different order", suspect the declared type of a member or param
  (DinoFieldChara::mUnk2D8 was MFx32Point, not MPoint: +3).
- Two adjacent member arrays addressed from one `this+off` register = both inside a nested struct member whose
  implicit ctor is inlined (`struct Ai { groups[3]; files[2]; } mAi;`).
- A by-value struct param copied with ldm/stm = a ctor-less POD struct local (`struct R {int l,t,r,b;} r = *(R *)&rect;`);
  MRect locals copy member-wise. A struct member split (`parts[4]; u32 unk14;`) reproduces a block copy + separate
  last word.
- Point copied field by field from another object's MPoint member: the by-value accessor
  (`MPoint font = mMarkMove->GetValue();`, SpotMove::Idle).
- Overloads matter: MObjCanvas has `const MPoint &` + `const u16 *` and by-value `MPoint` + `const void *` ctors.
- RAII objects show as adjacent saved values reloaded in the dtor (`MVramBankMap` in m/pal.cpp, MPalLoadTask::Transfer).
- Twin byte-identical `__vec_deleter<T>` dtors in one TU are D1 + D2.
- `T x(std::string(path));` is the most vexing parse: write `x((std::string(path)))`.

---

## Low-% functions

Below ~70% is usually a different expression shape, not junk. Try: one `new T(args)` expression; MRect/MPoint
helpers (`Intersects`, `Intersection`, `Moved`, `GetSize()`, `MPoint(0,0) - GetTopLeft()`); fewer named locals;
temps built inside static inline helpers; by-value accessors; inline item ctors (`#pragma always_inline on`); a
data-driven check of semantic scans (vtables, call targets, `new` sizes) for real logic bugs first.

---

## Dead ends

Proven unreproducible with the configured compiler; don't spend time without a genuinely new idea.

- **`this + i*4` kept on the stack across the path `std::string` dtor** (FieldMenuNumber, Fraction4, TeamTech,
  ReviveStatusDigit/Parts, TownMap ctors): identical output under all ten mwcc 2.0 builds, ipa on/off, opt pragmas,
  ref/ptr/int slot locals, chained assignment, helpers, type changes and the permuter.
- **Switch layouts that break the table rule** (View3D::OnMessage dispatch and hit-sound switch,
  UserController::OnMessage): case order, empty cases, value types, versions, `switch_tables` all fail.
- **ViewField::OnMessage `this` spill** (game spills an 83-use `this`): no level, flag or volatile reproduces it.
- **`DINO_COMMON_LAYOUT_INIT` sinits:** NV/volatile mixes change nothing (single-anchor aliasing blocks the reorder). (For ipa TUs see the Static init recipe.)
- **Function-local statics whose bss/data offset depends on data the TU doesn't emit** (LoadSlideInPict/SlideOutPict
  `sDirs`).
- **push_back of a non-trivial class T** (init_copy vs sub_push_back tag slots; View3D ctor, backup AddFile): header
  flips all fail.
- **ipa emission order where the game's callee follows its caller** (ResolveMove, keyboard
  MCreateBgCallTask<KasekiPictCache>): explicit instantiation before/after, auto_inline off, dont_inline all fail.
- MArchive::Release (lock promoted across calls), MMessageSubject::RemoveObserver (frame +8), MMenu::AddLink
  make_pair temp forms, deque `__cdeque_copy_in` mid copy, vector<char>::insert byte temp,
  MOamCanvas::GetPixel, text_codec/file.cpp/3d_resource .bss layouts.
- Two-slot swaps in 99.9x functions that use-count, declaration order, and per-loop counters didn't fix:
  ModelField/StatusEffectIcon Idle, EffectTelop StateWin, SelectAllMenu::OnStart.
- Things that are canonicalized (no effect): `==`/`<` operand swaps (but `u32 next = m84; if (m80 != next)` did fix
  one cmp order), `!x` spellings, `a*b` operand order, mwcc version (every 2.0 build is identical here),
  explicit instantiation placement in non-ipa TUs.
- Blind repo-wide idiom sweeps (op-assign flips, decl hoists, increment placement, return/break, erase copies,
  optimization levels) are exhausted: idioms pay where the asm shows the symptom.

---

## Unattended sweep

`tools/overnight_sweep.py` runs the permuter over the closest near-misses, gates every candidate and commits each
clean win on a new local branch; no LLM involved. Run it in a dedicated worktree with a clean `src/` and a built tree:

```sh
.venv/bin/python tools/overnight_sweep.py --dry-run --min 95 --exclude src/ov001 src/ov003   # list candidates
.venv/bin/python tools/overnight_sweep.py --min 95 --max-size 1500 -t 600 -j 6 --hours 8 \
    --units src/m src/game src/ov000 src/ov002 src/ov006 src/ov007                         # the night run
.venv/bin/python tools/overnight_sweep.py --mine --min 90 --max-size 4096                  # gate old outputs only
```

- It creates `sweep-<date>-<time>` (or `--branch NAME`; `--here` commits on the current HEAD), builds a baseline
  (`build/sweep/base.json`, `build/sweep/calls_base.txt`), then per function: permutes for `-t` seconds, gates the
  `--tries` lowest-score `best-*.cpp` outputs, reduces the winner hunk by hunk while its % and the gate hold, commits
  `<function> A->B: permuter candidate (best-N.cpp), gated by overnight_sweep`, and moves the baseline forward.
- Gate: build succeeds; no baseline 100% function drops; no other function loses more than `--fuzz` (0.001%); no new
  `tools/callcheck.py` line; the function gains at least `--min-gain` (0.005%).
- Log: `build/sweep/sweep.log`. `build/sweep/state.json` skips (function, file contents) pairs already tried; `--again`
  retries them, `--resume` starts the permuter from its best earlier output, `--only SYM...` limits the set.
- Ctrl-C restores the file being tried. Review every commit on the branch for semantics before merging it.
