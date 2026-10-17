# Thumb function discovery for code dsd's analysis missed.
# usage: analyze.py OV [--check]   (OV = 0, 6, 7 ...)
# Writes $SP/gap/ovNNN.json with new functions, size fixes and relocs.
import sys, re, json, struct, os, capstone

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
CFG = ROOT + '/config/usa/arm9'
SP = os.path.join(ROOT, 'build', 'gapfuncs'); os.makedirs(SP, exist_ok=True)

def sections(path):
    secs = {}
    for l in open(path):
        m = re.match(r'\s+(\.\w+)\s+start:0x([0-9a-f]+) end:0x([0-9a-f]+)', l)
        if m and m.group(1) not in secs:
            secs[m.group(1)] = (int(m.group(2), 16), int(m.group(3), 16))
        if l.strip() == '' and secs:
            break
    return secs

def funcs(path):
    out = {}
    for l in open(path):
        m = re.match(r'(\S+) kind:function\((\w+),size=0x([0-9a-f]+)(?:,unknown)?\) addr:0x([0-9a-f]+)', l)
        if m:
            out[int(m.group(4), 16)] = (int(m.group(3), 16), m.group(1))
    return out

def syms_all(path):
    out = {}
    for l in open(path):
        m = re.match(r'(\S+) kind:(\S+) addr:0x([0-9a-f]+)', l)
        if m:
            out.setdefault(int(m.group(3), 16), (m.group(1), m.group(2)))
    return out

OVS = list(range(8))
MODS = {}
MODS['main'] = dict(secs=sections(CFG + '/delinks.txt'), bin=open(ROOT + '/extract/usa/arm9/arm9.bin', 'rb').read(), base=0x02000000,
                    funcs=funcs(CFG + '/symbols.txt'))
for o in OVS:
    d = CFG + '/overlays/ov%03d' % o
    s = sections(d + '/delinks.txt')
    MODS[o] = dict(secs=s, bin=open(ROOT + '/extract/usa/arm9_overlays/ov%03d.bin' % o, 'rb').read(), base=s['.text'][0],
                   funcs=funcs(d + '/symbols.txt'))
for k, m in MODS.items():
    m['lo'] = min(a for a, b in m['secs'].values())
    m['hi'] = max(b for a, b in m['secs'].values())
ITCM = (0x01ff8000, 0x02000000)
DTCM = (0x027e0000, 0x027e4000)

def overlaps(a, b):
    return MODS[a]['lo'] < MODS[b]['hi'] and MODS[b]['lo'] < MODS[a]['hi']

def target_module(own, val, call=False):
    """module string for a pointer/call from overlay `own` to val, or None if it isn't an address."""
    if ITCM[0] <= val < ITCM[1]:
        return 'itcm'
    if DTCM[0] <= val < DTCM[1]:
        return 'dtcm'
    m = MODS['main']
    if m['lo'] <= val < m['hi']:
        return 'main'
    o = MODS[own]
    if o['lo'] <= val < o['hi']:
        return 'overlay(%d)' % own
    cands = [k for k in OVS if k != own and not overlaps(k, own) and MODS[k]['lo'] <= val < MODS[k]['hi']]
    if call:
        exact = [k for k in cands if (val & ~1) in MODS[k]['funcs']]
        if exact:
            cands = exact
    if not cands:
        return None
    if len(cands) == 1:
        return 'overlay(%d)' % cands[0]
    return 'overlays(%s)' % ','.join(map(str, cands))

md = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB)
md.detail = True

class Fail(Exception):
    pass

def u16(mod, a):
    return struct.unpack_from('<H', mod['bin'], a - mod['base'])[0]

def u32(mod, a):
    return struct.unpack_from('<I', mod['bin'], a - mod['base'])[0]

def decode(mod, a):
    off = a - mod['base']
    ins = next(md.disasm(mod['bin'][off:off + 4], a, 1), None)
    if ins is None:
        raise Fail('undecodable at %x' % a)
    return ins

def switch_count(mod, a):
    """Entry count from the index register's range check (cmp rI, #N; bhi) before `adds rX, rI, rI`, if simple."""
    try:
        idx = decode(mod, a - 10)
    except Fail:
        return None
    if idx.mnemonic != 'adds':
        return None
    ri = idx.op_str.split(', ')[1]
    for back in range(12, 26, 2):
        try:
            c = decode(mod, a - back)
        except Fail:
            return None
        if c.mnemonic == 'cmp' and c.op_str.startswith(ri + ', #') and c.size == 2:
            try:
                br = decode(mod, a - back + 2)
            except Fail:
                return None
            if br.mnemonic in ('bhi', 'bls', 'bhs', 'blo'):
                return int(c.op_str.split('#')[1], 0) + 1
            return None
    return None

def analyze(mod, start, limit):
    """Recursive descent over one thumb function. Returns (end, calls, pools, tables)."""
    seen = set()
    work = [start]
    pools = set()
    calls = []   # (from, to, kind)
    tables = []  # (start, end)
    tail = []
    pops = False
    code_end = start
    deferred = []
    def parse_table(a, count):
        tb = a + 2
        base = a + 4
        ents = []
        p = tb
        lo = None
        while True:
            if count is not None and len(ents) >= count:
                break
            if count is None and lo is not None and p >= lo:
                break
            if count is None and p in seen:
                break
            e = struct.unpack_from('<h', mod['bin'], p - mod['base'])[0]
            t = base + e
            if e & 1 or not (start <= t < limit):
                raise Fail('bad switch entry at %x' % p)
            ents.append(t)
            if t > p:
                lo = t if lo is None else min(lo, t)
            p += 2
            if len(ents) > 512:
                raise Fail('switch too long at %x' % a)
        return tb, p, ents
    while work or deferred:
        if not work:
            a = deferred.pop(0)
            tb, p, ents = parse_table(a, None)
            tables.append((tb, p))
            code_end = max(code_end, p)
            work.extend(ents)
            continue
        a = work.pop()
        while True:
            if a in seen:
                break
            if not (start <= a < limit):
                raise Fail('flow leaves function at %x' % a)
            if a in pools or any(s <= a < e for s, e in tables):
                raise Fail('flow into data at %x' % a)
            if u16(mod, a) == 0:
                raise Fail('0000 in code at %x' % a)
            ins = decode(mod, a)
            seen.add(a)
            nxt = a + ins.size
            code_end = max(code_end, nxt)
            mn = ins.mnemonic
            ops = ins.op_str
            if mn in ('bl', 'blx') and ops.startswith('#'):
                t = int(ops[1:], 16)
                calls.append((a, t, 'thumb_call' if mn == 'bl' else 'thumb_call_arm'))
                if mn == 'bl' and start < t < limit:
                    work.append(t)   # mwcc uses bl as a long branch inside a function
                a = nxt
                continue
            if mn == 'blx':  # register
                a = nxt
                continue
            if mn == 'b' and ops.startswith('#'):
                t = int(ops[1:], 16)
                if start <= t < limit:
                    work.append(t)
                else:
                    tail.append((a, t))
                break
            if mn.startswith('b') and mn[1:] in ('eq', 'ne', 'hs', 'lo', 'mi', 'pl', 'vs', 'vc', 'hi', 'ls', 'ge', 'lt', 'gt', 'le', 'cs', 'cc') and ops.startswith('#'):
                work.append(int(ops[1:], 16))
                a = nxt
                continue
            if mn == 'bx':
                break
            if mn == 'pop':
                pops = True
            if mn == 'pop' and 'pc' in ops:
                break
            if mn == 'ldr' and '[pc' in ops:
                m = re.search(r'\[pc, #(0x[0-9a-f]+|\d+)\]', ops)
                imm = int(m.group(1), 0) if m else 0
                p = ((a + 4) & ~3) + imm
                pools.add(p)
                a = nxt
                continue
            if mn == 'add' and ops.startswith('pc,'):
                # mwcc switch: adds rX, rI, rI; add rX, pc; ldrh rX, [rX, #6]; lsls; asrs; add pc, rX; .hword table
                cnt = switch_count(mod, a)
                if cnt is None:
                    deferred.append(a)
                else:
                    tb, p, ents = parse_table(a, cnt)
                    tables.append((tb, p))
                    code_end = max(code_end, p)
                    work.extend(ents)
                break
            if mn == 'mov' and ops.startswith('pc'):
                break
            a = nxt
    end = code_end
    for p in pools:
        end = max(end, p + 4)
    # every instruction must lie outside pools
    for p in pools:
        if p in seen or p + 2 in seen:
            raise Fail('pool %x overlaps code' % p)
    return dict(start=start, end=end, calls=calls, pools=sorted(pools), tables=tables, tail=tail, ninsn=len(seen), pops=pops, seen=seen)

def main():
    own = int(sys.argv[1])
    mod = MODS[own]
    ts, te = mod['secs']['.text']
    region_end = te
    if '.rodata' in mod['secs'] and mod['secs']['.rodata'][0] == te:
        region_end = mod['secs']['.rodata'][1]   # code dsd left as .rodata
    known = dict(mod['funcs'])
    if region_end != te:
        rp = CFG + '/overlays/ov%03d/relocs.txt' % own
        for l in open(rp):
            m = re.match(r'from:0x([0-9a-f]+) kind:load to:0x([0-9a-f]+)', l)
            if m:
                f, t = int(m.group(1), 16), int(m.group(2), 16)
                if f < te and te <= t < region_end and not (t & 1):
                    region_end = min(region_end, t)
    # seeds: thumb pointers in this overlay's bytes, and calls into the region from anywhere
    seeds = set()
    soft = set()
    b = mod['bin']
    for i in range(0, len(b) - 3, 4):
        v = struct.unpack_from('<I', b, i)[0]
        if v & 1 and ts <= v - 1 < region_end and (v - 1) % 4 == 0:
            seeds.add(v - 1)
    for path in [CFG + '/relocs.txt'] + [CFG + '/overlays/ov%03d/relocs.txt' % o for o in OVS]:
        for l in open(path):
            m = re.match(r'from:0x([0-9a-f]+) kind:(\S+) to:0x([0-9a-f]+) module:none', l)
            if m:
                t = int(m.group(3), 16) & ~1
                if ts <= t < region_end:
                    soft.add(t)
    result = dict(fix=[], new=[], fail=[], skipped=[], merged=[])
    # where each existing function is referenced from (relocs anywhere, plus raw pointers in this overlay)
    refs = {a: [] for a in known}
    for path, mname in [(CFG + '/relocs.txt', 'main')] + [(CFG + '/overlays/ov%03d/relocs.txt' % o, o) for o in OVS]:
        for l in open(path):
            m = re.match(r'from:0x([0-9a-f]+) kind:\S+ to:0x([0-9a-f]+) module:(\S+)', l)
            if not m:
                continue
            t = int(m.group(2), 16) & ~1
            if t in refs and ('overlay(%d)' % own == m.group(3) or ('overlays(' in m.group(3) and str(own) in m.group(3)) or m.group(3) == 'none'):
                refs[t].append(int(m.group(1), 16) if mname == own else -1)
    for i in range(0, len(b) - 3, 4):
        v = struct.unpack_from('<I', b, i)[0]
        if v & 1 and (v - 1) in refs and not (ts <= mod['base'] + i < te):
            refs[v - 1].append(-1)
    def internal(fn, owner):
        r = refs.get(fn, [])
        if not (r and all(owner <= f < fn for f in r)):
            return False
        i = decode(mod, fn)
        if i.mnemonic == 'push':
            return False
        # a long-branch label's code flows back into its owner
        try:
            res = analyze(mod, fn, limit_after(fn))
        except Fail as e:
            m = re.match(r'flow leaves function at ([0-9a-f]+)', str(e))
            return bool(m) and owner <= int(m.group(1), 16) < fn
        # or it pops a frame it never pushed
        return res['pops'] or any(owner <= t < fn for f, t in res['tail'])
    # re-check existing functions
    starts = sorted(set(known) | seeds)
    def limit_after(a):
        for s in starts:
            if s > a:
                return min(s, region_end)
        return region_end
    for a in sorted(known):
        if a not in known or not (ts <= a < region_end):
            continue
        size, name = known[a]
        lim = limit_after(a)
        skip = []
        while lim < region_end and lim in known and internal(lim, a) and lim not in seeds:
            skip.append(lim)
            lim = limit_after(lim)
        try:
            r = analyze(mod, a, lim if skip else limit_after(a))
        except Fail as e:
            result['fail'].append((hex(a), name, 'existing: ' + str(e)))
            continue
        if r['end'] > a + size:
            gone = [x for x in skip if x < r['end']]
            result['fix'].append(dict(addr=a, name=name, old=size, new=r['end'] - a, absorbs=[(hex(x), known[x][1]) for x in gone]))
            for x in gone:
                result['merged'].append((x, known[x][1]))
                del known[x]
            known[a] = (r['end'] - a, name)
            starts = sorted(set(known) | seeds)
    # sweep holes, re-running until the call targets found stop adding seeds
    def is_push_lr(a):
        try:
            i = decode(mod, a)
        except Fail:
            return False
        return i.mnemonic == 'push'
    while True:
        starts = sorted(set(known) | seeds)
        newf = {}
        skipped = []
        pos = ts
        ki = sorted((s, s + sz) for s, (sz, n) in known.items())
        def covered(a):
            for s, e in ki:
                if s <= a < e:
                    return e
            return None
        while pos < region_end:
            c = covered(pos)
            if c is not None:
                pos = (c + 3) & ~3
                continue
            if u16(mod, pos) == 0 and u16(mod, pos + 2) == 0:
                pos += 4
                continue
            lim = limit_after(pos)
            try:
                r = analyze(mod, pos, lim)
                newf[pos] = r
                pos = (r['end'] + 3) & ~3
            except Fail as e:
                nxt = min([x for x in starts if x > pos] + [region_end])
                skipped.append((hex(pos), hex(nxt), str(e)))
                pos = nxt
        added = set()
        for st, r in newf.items():
            for (f, t, k) in r['calls']:
                if k != 'thumb_call' or not (ts <= t < region_end) or t in known or t in seeds or t % 4:
                    continue
                if st <= t < r['end'] and not is_push_lr(t):
                    continue   # long branch inside the caller
                added.add(t)
        if not added:
            break
        seeds |= added
    result['skipped'] = skipped
    # seeds not hit
    allstarts = set(known) | set(newf)
    result['seeds_missed'] = [hex(s) for s in sorted(seeds) if s not in allstarts]
    result['soft'] = [(hex(s), s in allstarts) for s in sorted(soft)]
    for s, r in sorted(newf.items()):
        relocs = []
        for (f, t, k) in r['calls']:
            mm = target_module(own, t, call=True)
            relocs.append((f, k, t, mm))
        for p in r['pools']:
            v = u32(mod, p)
            mm = target_module(own, v)
            if mm:
                relocs.append((p, 'load', v, mm))
        result['new'].append(dict(addr=s, size=r['end'] - s, relocs=relocs, tail=r['tail'], ninsn=r['ninsn']))
    json.dump(result, open(SP + '/ov%03d.json' % own, 'w'), indent=1)
    print('ov%03d: fix %d, new %d (%#x bytes), fail %d, skipped %d, seeds missed %d, region end %#x' % (
        own, len(result['fix']), len(result['new']), sum(n['size'] for n in result['new']), len(result['fail']),
        len(result['skipped']), len(result['seeds_missed']), region_end))
    if newf:
        print('  last new function ends %#x' % max(r['end'] for r in newf.values()))

main()
