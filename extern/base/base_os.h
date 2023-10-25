#pragma once

#include "base_common.h"

#define OS_PROT_NONE 0x00  // PROT_NONE
#define OS_PROT_READ 0x01  // PROT_READ
#define OS_PROT_WRITE 0x02 // PROT_WRITE
#define OS_PROT_EXEC 0x04  // PROT_EXEC

// @Memory ==================================================================================

void *os_alloc(u64 size);
void *os_map_file(i32 file, u64 size, i32 offset);
i32 os_set_prot(void *ptr, u64 size, i32 flags);
void os_free(void *ptr, u64 size);
