#include "base_os.h"

#ifdef PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#ifdef PLATFORM_UNIX
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <sys/param.h>
#endif

#ifdef PLATFORM_MACOS
#include <mach-o/dyld.h>
#undef bool
#endif

#include <stdio.h>

// @Memory ///////////////////////////////////////////////////////////////////////////////

void *os_reserve_vm(void *addr, u64 size)
{
  void *result = NULL;
  
  #ifdef PLATFORM_WINDOWS
  result = VirtualAlloc(addr, size, MEM_RESERVE, PAGE_NOACCESS);
  #endif

  #ifdef PLATFORM_UNIX
  result = mmap(NULL, size, PROT_NONE, MAP_ANON | MAP_PRIVATE, -1, 0);
  if (result == MAP_FAILED)
  {
    printf("mmap failed with size %llu \n", size);
    assert(0);
  }
  #endif

  return result;
}

bool os_commit_vm(void *addr, u64 size)
{
  bool result = TRUE;

  #ifdef PLATFORM_WINDOWS
  byte *ptr = VirtualAlloc(addr, size, MEM_COMMIT, PAGE_READWRITE);
  if (ptr == NULL)
  {
    result = GetLastError();
  }
  #endif

  #ifdef PLATFORM_UNIX
  i32 err = mprotect(addr, size, PROT_READ | PROT_WRITE);
  if (err != 0)
  {
    printf("Failed to commit with mprotect.\n");
    result = FALSE;
  }

  #endif

  return result;
}

bool os_decommit_vm(void *addr, u64 size)
{
  bool result = TRUE;

  #ifdef PLATFORM_WINDOWS
  result = VirtualFree(addr, size, MEM_DECOMMIT);
  #endif

  #ifdef PLATFORM_UNIX
  i32 err = mprotect(addr, size, PROT_NONE);
  if (err != 0)
  {
    printf("Failed to decommit with mprotect.\n");
    result = FALSE;
  }
  #endif

  return result;
}

void os_release_vm(void *ptr, u64 size)
{
  #ifdef PLATFORM_WINDOWS
  VirtualFree(ptr, size, MEM_RELEASE);
  #endif

  #ifdef PLATFORM_UNIX
  munmap(ptr, size);
  #endif
}

// TODO(dg): This should be cached somewhere
u64 os_get_page_size(void)
{
  u64 result = 0;

  #ifdef PLATFORM_WINDOWS
  SYSTEM_INFO info = {0};
  GetSystemInfo(&info);
  result = info.dwPageSize;
  #endif

  #ifdef PLATFORM_UNIX
  result = getpagesize();
  #endif

  return result;
}
// @File /////////////////////////////////////////////////////////////////////////////////

bool os_is_handle_valid(OS_Handle handle)
{
  bool result = FALSE;

  #ifdef PLATFORM_WINDOWS
  result = (void *) handle.id != INVALID_HANDLE_VALUE;
  #endif

  #ifdef PLATFORM_UNIX
  #endif

  return result;
}

OS_Handle os_open_file(String path, OS_Flag flag)
{
  OS_Handle result = {0};

  #ifdef PLATFORM_WINDOWS
  b32 access;
  if (flag == OS_FILE_READ)
  {
    access = GENERIC_READ;
  }
  else if (flag == OS_FILE_WRITE)
  {
    access = GENERIC_WRITE;
  }
  else if (flag == (OS_FILE_READ | OS_FILE_WRITE))
  {
    access = GENERIC_READ | GENERIC_WRITE;
  }
  else
  {
    return (OS_Handle) {0};
  }

  HANDLE handle = CreateFileA(path.data, 
                              access, 
                              FILE_SHARE_READ | FILE_SHARE_WRITE, 
                              NULL, OPEN_EXISTING, 
                              FILE_ATTRIBUTE_NORMAL, 
                              NULL);
  result.id = (u64) handle;
  #endif

  #ifdef PLATFORM_UNIX
  b32 access;
  if (flag == OS_FILE_READ)
  {
    access = O_RDONLY;
  }
  else if (flag == OS_FILE_WRITE)
  {
    access = O_WRONLY;
  }
  else if (flag == (OS_FILE_READ | OS_FILE_WRITE))
  {
    access = O_RDWR;
  }
  else
  {
    return (OS_Handle) {0};
  }

  result.id = open(path.data, access);
  #endif

  return result;
}

void os_close_file(OS_Handle file)
{
  #ifdef PLATFORM_WINDOWS
  HANDLE handle = (HANDLE) file.id;
  CloseHandle(handle);
  #endif

  #ifdef PLATFORM_UNIX
  close(file.id);
  #endif
}

String os_read_file(OS_Handle file, u64 size, u64 pos, Arena *arena)
{
  assert(pos + size <= 0xffffffff);

  if (!os_is_handle_valid(file)) return (String) {0};

  String result = {0};
  result.data = (char *) arena_push(arena, i8, size);

  #ifdef PLATFORM_WINDOWS
  HANDLE handle = (HANDLE) file.id;
  SetFilePointer(handle, pos, NULL, FILE_BEGIN);
  ReadFile(handle, result.data, size, (unsigned long *) &result.len, NULL);
  #endif

  #ifdef PLATFORM_UNIX
  lseek(file.id, pos, SEEK_SET);
  result.len = read(file.id, result.data, size);
  #endif

  return result;
}

void os_write_file(OS_Handle file, String buf)
{
  assert(buf.len <= 0xffffffff);

  if (!os_is_handle_valid(file)) return;

  #ifdef PLATFORM_WINDOWS
  HANDLE handle = (HANDLE) file.id;
  WriteFile(handle, buf.data, buf.len, NULL, NULL);
  #endif

  #ifdef PLATFORM_UNIX
  write(file.id, buf.data, buf.len);
  #endif
}

void os_set_file_pos(OS_Handle file, u64 pos)
{
  if (!os_is_handle_valid(file)) return;

  #ifdef PLATFORM_WINDOWS
  HANDLE handle = (HANDLE) file.id;
  SetFilePointer(handle, pos, NULL, FILE_BEGIN);
  #endif

  #ifdef PLATFORM_UNIX
  lseek(file.id, pos, SEEK_SET);
  #endif
}

#ifdef __APPLE__
String os_path_to_executable(String name)
{
  char buf[MAXPATHLEN];
  u32 size = MAXPATHLEN;
  _NSGetExecutablePath(buf, &size);
  String path = (String) {buf, size};
  i64 loc = str_find(path, name, 0, size);
  path = str_substr(path, 0, loc);

  return path;
}
#endif

OS_Handle os_handle_to_stdin(void)
{
  OS_Handle result = {0};

  #ifdef PLATFORM_WINDOWS
  result.id = (u64) GetStdHandle(STD_INPUT_HANDLE);
  #endif

  #ifdef PLATFORM_UNIX
  result.id = STDIN_FILENO;
  #endif

  return result;
}

OS_Handle os_handle_to_stdout(void)
{
  OS_Handle result = {0};

  #ifdef PLATFORM_WINDOWS
  result.id = (u64) GetStdHandle(STD_OUTPUT_HANDLE);
  #endif

  #ifdef PLATFORM_UNIX
  result.id = STDOUT_FILENO;
  #endif

  return result;
}

OS_Handle os_handle_to_stderr(void)
{
  OS_Handle result = {0};

  #ifdef PLATFORM_WINDOWS
  result.id = (u64) GetStdHandle(STD_ERROR_HANDLE);
  #endif

  #ifdef PLATFORM_UNIX
  result.id = STDERR_FILENO;
  #endif

  return result;
}

#ifdef PLATFORM_WINDOWS
inline
void os_windows_output_debug(const char *cstr)
{
  OutputDebugStringA(cstr);
}
#endif
