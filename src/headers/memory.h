#ifndef MEMORY_GUARD
#define MEMORY_GUARD

#include<stdio.h>
#include<headers/cpu.h>

#define PHYSICAL_MEMORY_SPACE 65535
#define MAX_INDEX_PHYSICAL_PAGE 15

uint8_t pm[PHYSICAL_MEMORY_SPACE];

uint64_t read64bits_dram(uint64_t paddr, core_t *cr);
void write64bits_dram(uint64_t paddr, uint64_t data, core_t *cr);

#endif