// Sections for code and data that are copied into the ARM9's tightly-coupled memories at boot.
#ifndef NITRO_SECTION_H
#define NITRO_SECTION_H

#pragma define_section ITCM ".itcm" ".itcm.bss" abs32 RWX
#pragma define_section DTCM ".dtcm" ".dtcm.bss" abs32 RWX

#endif
