#include <stdlib.h>

#include "../base/base.h"
#include "os.h"

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

void *os_alloc(u64 size)
{
  void *result = NULL;
  
  #ifdef PLATFORM_WINDOWS
  // VirtualAlloc(NULL, size, );
  #endif

  #ifdef PLATFORM_UNIX
  // i32 p_flags = PROT_READ | PROT_WRITE | PROT_EXEC;
  // i32 m_flags = MAP_ANON | MAP_SHARED;
  // result = mmap(NULL, size, p_flags, m_flags, -1, 0);
  #endif
  
  result = malloc(size);

  return result;
}

void os_free(void *ptr, u64 size)
{
  #ifdef PLATFORM_WINDOWS
  // VirtualFree();
  #endif

  #ifdef PLATFORM_UNIX
  // munmap(ptr, size);
  #endif

  free(ptr);
}

// @File /////////////////////////////////////////////////////////////////////////////////

OS_Handle os_open(String path, OS_Flag flag)
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
  assert(pos + size <= UINT_MAX);

  String result = {0};
  result.data = arena_push(arena, size);

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

void os_write(OS_Handle file, String buf)
{
  assert(buf.len <= UINT_MAX);

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
  HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);
  result.id = (u64) handle;
  #endif

  return result;
}

OS_Handle os_handle_to_stdout(void)
{
  OS_Handle result = {0};

  #ifdef PLATFORM_WINDOWS
  HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
  result.id = (u64) handle;
  #endif

  return result;
}

OS_Handle os_handle_to_stderr(void)
{
  OS_Handle result = {0};

  #ifdef PLATFORM_WINDOWS
  HANDLE handle = GetStdHandle(STD_ERROR_HANDLE);
  result.id = (u64) handle;
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
