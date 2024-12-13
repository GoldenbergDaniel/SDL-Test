#pragma once

#include "base_common.h"
#include "base_string.h"

// @Memory ///////////////////////////////////////////////////////////////////////////////

void *os_reserve_vm(void *addr, u64 size);
bool os_commit_vm(void *addr, u64 size);
bool os_decommit_vm(void *addr, u64 size);
void os_release_vm(void *addr, u64 size);

u64 os_get_page_size(void);

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
  i64 id;
};

bool os_is_handle_valid(OS_Handle handle);

OS_Handle os_open_file(String path, OS_Flag flag);
void os_close_file(OS_Handle file);
String os_read_file(OS_Handle file, u64 size, u64 pos, Arena *arena);
void os_write_file(OS_Handle handle, String buf);

OS_Handle os_handle_to_stdin(void);
OS_Handle os_handle_to_stdout(void);
OS_Handle os_handle_to_stderr(void);

#ifdef PLATFORM_WINDOWS
void os_windows_output_debug(const char *cstr);
#endif

String os_path_to_executable(String name);
