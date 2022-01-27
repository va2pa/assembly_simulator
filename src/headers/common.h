#ifndef DEBUG_GUARD
#define DEBUG_GUARD

#include<stdint.h>

#define DEBUG_INSTRUCTIONCYCLE     0x1
#define DEBUG_REGISTERS         0x2
#define DEBUG_PRINTSTACK        0x4
#define DEBUG_PRINTCACHESET     0x8
#define DEBUG_CACHEDETAILS      0x10
#define DEBUG_MMU               0x20
#define DEBUG_LNKER             0x40
#define DEBUG_LOADER            0x80
#define DEBUG_PARSEINST         0x100

#define DEBUG_VERBOSE_SET       0x1

#define DEBUG_ENABLE_SRAM_CACHE     0

uint64_t debug_printf(uint64_t open_set, const char *format, ...);

uint64_t string2uint(const char *str);
uint64_t string2uint_range(const char *str, int start, int end);

#endif