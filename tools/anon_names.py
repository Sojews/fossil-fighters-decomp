#!/usr/bin/env python3
"""Gives the compiler's anonymous data symbols the names the game's objects use, so objdiff can compare them.

Usage: python3 tools/anon_names.py OURS.o TARGET.o

mwcc names the constants it makes up itself (string literals, pointer-to-member constants, the templates of
aggregate initializers) @1, @2, ... dsd names the same data after its address (data_020c46c4). objdiff compares
relocations by symbol name, so a function that loads one never matches. For each function that is in both
objects with the same size, this pairs up their relocations by offset and renames our anonymous symbols to the
target's names, in place. It only touches the objects objdiff reads for units that aren't complete yet; linked
objects keep the compiler's names.
"""
import struct
import sys

SHT_SYMTAB = 2
SHT_RELA = 4
STT_FUNC = 2


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
                                 "shndx": shndx, "offset": off})

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
                relocs.append((off, info >> 8, info & 0xFF, addend))
        for relocs in out.values():
            relocs.sort()
        return out

    def functions(self):
        return {s["name"]: s for s in self.symbols if s["type"] == STT_FUNC and s["size"]}

    def rename(self, renames):
        """Renames symbols (index -> new name), appending the names to a new string table at the end."""
        strtab = self.sections[self.strtab_index]
        table = bytearray(self.data[strtab[4]:strtab[4] + strtab[5]])
        for index, name in renames.items():
            sym = self.symbols[index]
            struct.pack_into("<I", self.data, sym["offset"], len(table))
            table += name.encode("latin-1") + b"\0"
        while len(self.data) % 4:
            self.data.append(0)
        strtab[4], strtab[5] = len(self.data), len(table)
        self.data += table
        struct.pack_into("<IIIIIIIIII", self.data, self.shoff + self.strtab_index * self.shentsize, *strtab)
        open(self.path, "wb").write(self.data)


def main():
    ours, target = Elf(sys.argv[1]), Elf(sys.argv[2])
    our_relocs, target_relocs = ours.relocations(), target.relocations()
    target_funcs = target.functions()
    renames = {}
    for name, func in ours.functions().items():
        tfunc = target_funcs.get(name)
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
            if sym["name"].startswith("@") and not tsym["name"].startswith("@") and a[1] not in renames:
                renames[a[1]] = tsym["name"]
    if renames:
        ours.rename(renames)


if __name__ == "__main__":
    main()
