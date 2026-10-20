#!/usr/bin/env python3
"""Gives the compiler's anonymous data symbols the names the game's objects use, so objdiff can compare them.

Usage: python3 tools/anon_names.py OURS.o TARGET.o

mwcc names the constants it makes up itself (string literals, pointer-to-member constants, the templates of
aggregate initializers) @1, @2, ... dsd names the same data after its address (data_020c46c4). objdiff compares
relocations by symbol name, so a function that loads one never matches. For each function that is in both
objects with the same size, this pairs up their relocations by offset and renames our anonymous symbols to the
target's names, in place.

With interprocedural analysis (-ipa file) mwcc also addresses a file's own data through its sections (`.data+0x80`,
`.bss+0`) instead of through symbols. dsd names the same places (data_020c45f8, gLooper), so such relocations are
pointed at a symbol of that name at the same place: ours if there is one, or a new local one.

dsd also splits one block of data into symbols every so often (a TU's pooled constants every 0x80 bytes), while the
source declares the block once: our `data_02172f78+0x80` is the game's `data_02172ff8`. Such a relocation is pointed at
an (undefined) symbol of the game's name.
"""
import re
import struct
import sys

SHT_SYMTAB = 2
SHT_RELA = 4
STT_FUNC = 2
STT_SECTION = 3


class Elf:
    def __init__(self, path):
        self.path = path
        self.data = bytearray(open(path, "rb").read())
        d = self.data
        self.shoff = struct.unpack_from("<I", d, 0x20)[0]
        self.shentsize, self.shnum, self.shstrndx = struct.unpack_from("<HHH", d, 0x2E)
        self.sections = [list(struct.unpack_from("<IIIIIIIIII", d, self.shoff + i * self.shentsize))
                         for i in range(self.shnum)]
        self.symtab_index = next(i for i, s in enumerate(self.sections) if s[1] == SHT_SYMTAB)
        symtab = self.sections[self.symtab_index]
        self.strtab_index = symtab[6]
        self.symbols = []
        for i in range(symtab[5] // 16):
            off = symtab[4] + i * 16
            name, value, size, info, other, shndx = struct.unpack_from("<IIIBBH", d, off)
            self.symbols.append({"name": self.string(name), "value": value, "size": size, "type": info & 0xF,
                                 "info": info, "shndx": shndx, "offset": off})

    def string(self, offset):
        strtab = self.sections[self.strtab_index]
        start = strtab[4] + offset
        return self.data[start:self.data.index(b"\0", start)].decode("latin-1")

    def relocations(self):
        """section index -> sorted [(offset, symbol index, type, addend)]"""
        out = {}
        for s in self.sections:
            if s[1] != SHT_RELA or s[6] != self.symtab_index:
                continue
            relocs = out.setdefault(s[7], [])
            for i in range(s[5] // 12):
                off, info, addend = struct.unpack_from("<IIi", self.data, s[4] + i * 12)
                relocs.append((off, info >> 8, info & 0xFF, addend, s[4] + i * 12))
        for relocs in out.values():
            relocs.sort()
        return out

    def functions(self):
        return {s["name"]: s for s in self.symbols if s["type"] == STT_FUNC and s["size"]}

    def rewrite(self, renames, new_symbols, retargets):
        """Renames symbols (index -> new name), adds symbols ((name, section, value)) and points relocations
        (file offset -> (symbol index, addend)) elsewhere. The symbol and string tables are rewritten at the end."""
        strtab = self.sections[self.strtab_index]
        table = bytearray(self.data[strtab[4]:strtab[4] + strtab[5]])
        for index, name in renames.items():
            struct.pack_into("<I", self.data, self.symbols[index]["offset"], len(table))
            table += name.encode("latin-1") + b"\0"
        symtab = self.sections[self.symtab_index]
        entries = bytearray(self.data[symtab[4]:symtab[4] + symtab[5]])
        for name, shndx, value in new_symbols:
            # Undefined ones are global (data in other objects); others local.
            entries += struct.pack("<IIIBBH", len(table), value, 0, 0x10 if shndx == 0 else 0, 0, shndx)
            table += name.encode("latin-1") + b"\0"
        for off, (index, addend) in retargets.items():
            info = struct.unpack_from("<I", self.data, off + 4)[0]
            struct.pack_into("<Ii", self.data, off + 4, (index << 8) | (info & 0xFF), addend)
        for section, blob in ((strtab, table), (symtab, entries)):
            while len(self.data) % 4:
                self.data.append(0)
            section[4], section[5] = len(self.data), len(blob)
            self.data += blob
        for i in (self.strtab_index, self.symtab_index):
            struct.pack_into("<IIIIIIIIII", self.data, self.shoff + i * self.shentsize, *self.sections[i])
        open(self.path, "wb").write(self.data)


DATA_NAME = re.compile(r"(data_(?:ov\d{3}_)?)([0-9a-f]{8})$")


def data_address(name, addend):
    """(prefix, address) for a dsd data symbol name plus an addend, or None."""
    m = DATA_NAME.match(name)
    return (m.group(1), int(m.group(2), 16) + addend) if m else None


def main():
    ours, target = Elf(sys.argv[1]), Elf(sys.argv[2])
    our_relocs, target_relocs = ours.relocations(), target.relocations()
    target_funcs = target.functions()
    renames = {}
    new_symbols = []   # (name, section, value)
    retargets = {}     # reloc file offset -> (symbol index, addend)
    # A static function whose name another file's function also has is named after its address in the game
    # (ForceStopStrm_02070314), and a weak template instance another overlay also has after its overlay
    # (..._ov002): give ours that name when it is the only such function, or else when exactly one such function
    # has the same size (a nonmatching function's size can differ).
    func_names = {}
    for name, func in ours.functions().items():
        func_names[name] = name
        if name in target_funcs:
            continue
        found = [t for t in target_funcs if re.fullmatch(re.escape(name) + r"_(?:[0-9a-f]{8}|ov\d{3})", t)]
        if len(found) > 1:
            found = [t for t in found if target_funcs[t]["size"] == func["size"]]
        if len(found) == 1:
            renames[ours.symbols.index(func)] = func_names[name] = found[0]
    for name, func in ours.functions().items():
        tfunc = target_funcs.get(func_names[name])
        if tfunc is None or tfunc["size"] != func["size"]:
            continue
        mine = [r for r in our_relocs.get(func["shndx"], [])
                if func["value"] <= r[0] < func["value"] + func["size"]]
        theirs = [r for r in target_relocs.get(tfunc["shndx"], [])
                  if tfunc["value"] <= r[0] < tfunc["value"] + tfunc["size"]]
        if len(mine) != len(theirs):
            continue
        for a, b in zip(mine, theirs):
            if a[0] - func["value"] != b[0] - tfunc["value"] or a[2] != b[2]:
                break
            sym, tsym = ours.symbols[a[1]], target.symbols[b[1]]
            if tsym["name"].startswith("@") or not tsym["name"] or tsym["type"] == STT_SECTION:
                continue
            if sym["name"].startswith("@") and a[1] not in renames:
                renames[a[1]] = tsym["name"]
            elif (sym["name"] != tsym["name"] and data_address(sym["name"], a[3]) is not None
                  and data_address(sym["name"], a[3]) == data_address(tsym["name"], b[3])):
                key = (tsym["name"], 0, 0)
                index = next((i for i, s in enumerate(ours.symbols) if s["name"] == tsym["name"]), None)
                if index is None:
                    if key not in new_symbols:
                        new_symbols.append(key)
                    index = len(ours.symbols) + new_symbols.index(key)
                retargets[a[4]] = (index, b[3])
            elif sym["type"] == STT_SECTION:
                # Our section + addend is the target's symbol + its addend.
                value = a[3] - b[3]
                key = (tsym["name"], sym["shndx"], value)
                index = next((i for i, s in enumerate(ours.symbols) if s["name"] == tsym["name"]
                              and s["shndx"] == sym["shndx"] and s["value"] == value and s["type"] != STT_SECTION),
                             None)
                if index is None:
                    if key not in new_symbols:
                        new_symbols.append(key)
                    index = len(ours.symbols) + new_symbols.index(key)
                retargets[a[4]] = (index, b[3])
    if renames or new_symbols or retargets:
        ours.rewrite(renames, new_symbols, retargets)


if __name__ == "__main__":
    main()
