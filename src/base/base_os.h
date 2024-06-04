#pragma once

#include "base_common.h"
#include "base_string.h"

// @Memory ///////////////////////////////////////////////////////////////////////////////

void *os_alloc(u64 size);
void os_free(void *ptr, u64 size);

String os_path_to_executable(String name);

// @File /////////////////////////////////////////////////////////////////////////////////

typedef enum OS_Flag
{
  OS_FILE_READ = 1 << 0,
  OS_FILE_WRITE = 1 << 1,
  OS_FILE_CREATE = 1 << 2,
} OS_Flag;

typedef struct OS_Handle OS_Handle;
struct OS_Handle
{
  u64 data[1];
};

OS_Handle os_open(String path, OS_Flag flag);
void os_close(OS_Handle handle);
String os_read(OS_Handle handle, u64 size, u64 pos, Arena *arena);
void os_write(OS_Handle handle, String buf);
