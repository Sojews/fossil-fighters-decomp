# Apply analyze.py results to the dsd config: symbols, relocs, section bounds.
# usage: apply.py [--write] OV...
import sys, re, json, struct, os

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
CFG = ROOT + '/config/usa/arm9'
SP = os.path.join(ROOT, 'build', 'gapfuncs'); os.makedirs(SP, exist_ok=True)
sys.argv, WRITE = [a for a in sys.argv if a != '--write'], '--write' in sys.argv
OVLIST = [int(x) for x in sys.argv[1:]]

def modpath(m, f):
    return CFG + '/' + f if m == 'main' else CFG + '/overlays/ov%03d/%s' % (m, f)

def sections(path):
    secs = {}
    for l in open(path):
        m = re.match(r'\s+(\.\w+)\s+start:0x([0-9a-f]+) end:0x([0-9a-f]+)', l)
        if m and m.group(1) not in secs:
            secs[m.group(1)] = (int(m.group(2), 16), int(m.group(3), 16))
        if l.strip() == '' and secs:
            break
    return secs

class Syms:
    def __init__(self, m):
        self.m = m
        self.path = modpath(m, 'symbols.txt')
        self.lines = open(self.path).read().splitlines()
        self.secs = sections(modpath(m, 'delinks.txt'))
    def addrs(self):
        out = {}
        for i, l in enumerate(self.lines):
            if l is None:
                continue
            mm = re.search(r'addr:0x([0-9a-f]+)', l)
            if mm:
                out.setdefault(int(mm.group(1), 16), []).append(i)
        return out
    def write(self):
        open(self.path, 'w').write('\n'.join(l for l in self.lines if l is not None) + '\n')

def parse_reloc(l):
    m = re.match(r'from:0x([0-9a-f]+) kind:(\S+) to:0x([0-9a-f]+)( add:(\S+))? module:(\S+)', l)
    return int(m.group(1), 16), m.group(2), int(m.group(3), 16), int(m.group(5), 0) if m.group(5) else 0, m.group(6)

def fmt_reloc(f, k, t, mod):
    return 'from:0x%08x kind:%s to:0x%08x module:%s' % (f, k, t, mod)

SYMS = {'main': Syms('main')}
for o in range(8):
    SYMS[o] = Syms(o)

def module_key(modstr):
    if modstr == 'main':
        return 'main'
    m = re.match(r'overlay\((\d+)\)', modstr)
    return int(m.group(1)) if m else None

pending_syms = {}   # module -> {addr: line}
needs = []
RELS = {}

def need_symbol(modstr, t):
    k = module_key(modstr)
    if k is None:
        return
    s = SYMS[k]
    if not hasattr(s, '_addrs'):
        s._addrs = s.addrs()
    if t in s._addrs or (t & ~1) in s._addrs or t in pending_syms.get(k, {}) or (t & ~1) in pending_syms.get(k, {}):
        return
    sec = None
    for name, (lo, hi) in s.secs.items():
        if lo <= t < hi:
            sec = name
    pre = 'data_' if k == 'main' else 'data_ov%03d_' % k
    if sec == '.bss':
        line = '%s%08x kind:bss addr:0x%08x' % (pre, t, t)
    else:
        line = '%s%08x kind:data(any) addr:0x%08x' % (pre, t, t)
    pending_syms.setdefault(k, {})[t] = line

stats = {}
for own in OVLIST:
    r = json.load(open(SP + '/ov%03d.json' % own))
    sy = SYMS[own]
    addrs = sy.addrs()
    st = stats.setdefault(own, {})
    bin_ = open(ROOT + '/extract/usa/arm9_overlays/ov%03d.bin' % own, 'rb').read()
    base = sy.secs['.text'][0]
    # code ranges whose relocs we own
    ranges = []   # (start, end, relocs, labels)
    for f in r['fix']:
        ranges.append((f['addr'], f['addr'] + f['new']))
    for n in r['new']:
        ranges.append((n['addr'], n['addr'] + n['size']))
    newfun = {n['addr']: n for n in r['new']}
    # --- symbols
    for x, name in r['merged']:
        for i in addrs.get(x, []):
            if 'kind:function' in sy.lines[i]:
                sy.lines[i] = '.L_%08x kind:label(thumb) addr:0x%08x local' % (x, x)
                st['merged'] = st.get('merged', 0) + 1
    for f in r['fix']:
        for i in addrs.get(f['addr'], []):
            if 'kind:function' in sy.lines[i]:
                sy.lines[i] = re.sub(r'size=0x[0-9a-f]+', 'size=0x%x' % f['new'], sy.lines[i])
    removed = 0
    for a, idxs in addrs.items():
        for (s, e) in ranges:
            if s <= (a & ~1) < e and (a & ~1) not in newfun or (a & ~1) in newfun and a in addrs and any('kind:data' in sy.lines[i] for i in idxs if sy.lines[i]):
                pass
        for i in idxs:
            l = sy.lines[i]
            if l is None or 'kind:data' not in l and 'kind:bss' not in l:
                continue
            if any(s <= (a & ~1) < e for s, e in ranges):
                sy.lines[i] = None
                removed += 1
    st['data syms removed'] = removed
    labels = set()
    for n in r['new']:
        sy.lines.append('func_ov%03d_%08x kind:function(thumb,size=0x%x) addr:0x%08x' % (own, n['addr'], n['size'], n['addr']))
        for (f, k, t, mm) in n['relocs']:
            if k == 'thumb_call' and n['addr'] < t < n['addr'] + n['size']:
                labels.add(t)
    # --- relocs
    rpath = modpath(own, 'relocs.txt')
    rel = {}
    for l in open(rpath).read().splitlines():
        if l.strip():
            rel[parse_reloc(l)[0]] = l
    mine = {}
    for n in r['new']:
        for (f, k, t, mm) in n['relocs']:
            if mm is None:
                continue
            mine[f] = (k, t, mm)
    # relocs of the code added to resized functions: re-derive with analyze's helpers
    if r['fix']:
        sys.path.insert(0, SP)
        src = open(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'analyze.py')).read().replace('\nmain()\n', '\n')
        g = {'__name__': 'gapmod', '__file__': os.path.join(os.path.dirname(os.path.abspath(__file__)), 'analyze.py')}
        exec(src, g)
        for fx in r['fix']:
            res = g['analyze'](g['MODS'][own], fx['addr'], fx['addr'] + fx['new'])
            for (f, t, k) in res['calls']:
                mine.setdefault(f, (k, t, g['target_module'](own, t, call=True)))
                if k == 'thumb_call' and fx['addr'] < t < fx['addr'] + fx['new']:
                    labels.add(t)
            for p in res['pools']:
                v = struct.unpack_from('<I', bin_, p - base)[0]
                mm = g['target_module'](own, v)
                if mm:
                    mine.setdefault(p, ('load', v, mm))
    dropped = added = kept = 0
    for f in list(rel):
        if any(s <= f < e for s, e in ranges) and f not in mine:
            del rel[f]
            dropped += 1
    for f, (k, t, mm) in mine.items():
        if f in rel:
            of, ok, ot, oadd, omod = parse_reloc(rel[f])
            if ot + oadd == t or ot == t:
                kept += 1
                continue
        rel[f] = fmt_reloc(f, k, t, mm)
        added += 1
        if not (k == 'thumb_call' and module_key(mm) == own and (t in labels)):
            needs.append((own, f, mm, t))
    for t in labels:
        if t not in addrs:
            sy.lines.append('.L_%08x kind:label(thumb) addr:0x%08x local' % (t, t))
    # data words pointing at new functions (vtables, tables) outside code
    dataptr = 0
    newset = set(newfun)
    codes = sorted(ranges)
    for i in range(0, len(bin_) - 3, 4):
        a = base + i
        v = struct.unpack_from('<I', bin_, i)[0]
        if v & 1 and (v - 1) in newset and a not in rel and not any(s <= a < e for s, e in codes):
            rel[a] = fmt_reloc(a, 'load', v, 'overlay(%d)' % own)
            dataptr += 1
    st.update(dict(relocs_dropped=dropped, relocs_added=added, relocs_kept=kept, data_ptrs=dataptr, labels=len(labels), new=len(r['new'])))
    RELS[own] = (rpath, rel)
    # --- section bounds
    newend = max([e for s, e in ranges] + [sy.secs['.text'][1]])
    st['text_end'] = (hex(sy.secs['.text'][1]), hex(newend))
    if WRITE and newend != sy.secs['.text'][1]:
        dp = modpath(own, 'delinks.txt')
        d = open(dp).read()
        te = sy.secs['.text'][1]
        ro = sy.secs.get('.rodata')
        d = d.replace('.text       start:0x%08x end:0x%08x' % (sy.secs['.text'][0], te), '.text       start:0x%08x end:0x%08x' % (sy.secs['.text'][0], newend), 1)
        if ro and ro[0] == te:
            old = '    .rodata     start:0x%08x end:0x%08x kind:rodata align:4\n' % ro
            assert old in d, old
            if newend >= ro[1]:
                d = d.replace(old, '', 1)
            else:
                d = d.replace(old, '    .rodata     start:0x%08x end:0x%08x kind:rodata align:4\n' % (newend, ro[1]), 1)
        open(dp, 'w').write(d)

import bisect
symcache = {}
def sym_table(k):
    if k not in symcache:
        s = SYMS[k]
        al = sorted((a, s.lines[i]) for a, idx in s.addrs().items() for i in idx
                    if s.lines[i] and 'kind:label' not in s.lines[i])
        symcache[k] = ([a for a, l in al], al)
    return symcache[k]
addform = 0
for own, f, mm, t in needs:
    k = module_key(mm)
    if k is None:
        continue
    keys, al = sym_table(k)
    j = bisect.bisect_right(keys, t) - 1
    if j >= 0 and keys[j] == t:
        continue
    if j >= 0 and keys[j] == (t & ~1) and 'kind:function' in al[j][1]:
        continue
    a = keys[j]
    rpath, rel = RELS[own]
    ff, kk, tt, add, m2 = parse_reloc(rel[f])
    rel[f] = 'from:0x%08x kind:%s to:0x%08x add:%#x module:%s' % (f, kk, a, t - a, m2)
    addform += 1
print('add-form relocs', addform)
if WRITE:
    for own, (rpath, rel) in RELS.items():
        open(rpath, 'w').write('\n'.join(rel[k] for k in sorted(rel)) + '\n')

# main's unresolved calls into these overlays
mainrel = modpath('main', 'relocs.txt')
lines = open(mainrel).read().splitlines()
fixed = 0
for i, l in enumerate(lines):
    if 'module:none' not in l:
        continue
    f, k, t, add, mm = parse_reloc(l)
    hits = [o for o in OVLIST if (t & ~1) in {n['addr'] for n in json.load(open(SP + '/ov%03d.json' % o))['new']}]
    if len(hits) == 1:
        lines[i] = l.replace('module:none', 'module:overlay(%d)' % hits[0])
        fixed += 1
print('main module:none resolved', fixed)
for k, v in stats.items():
    print('ov%03d' % k, v)
if WRITE:
    open(mainrel, 'w').write('\n'.join(lines) + '\n')
    for k in SYMS:
        SYMS[k].write()

