#if defined(_WIN32) 
#define BACKEND_WINDOWS
#elif defined(_UNIX)
#define BACKEND_UNIX
#endif

#ifdef BACKEND_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#ifdef BACKEND_UNIX
#include <sys/mman.h>
#include <sys/dir.h>
#include <sys/param.h>
#endif

#ifdef __APPLE__
#include <mach-o/dyld.h>
#undef bool
#endif

#include <stdlib.h>

#include "base_common.h"
#include "base_os.h"

void *os_alloc(u64 size)
{
  void *result = NULL;
  
  #ifdef BACKEND_WINDOWS
  // VirtualAlloc(NULL, size, );
  #endif

  #ifdef BACKEND_UNIX
  // i32 p_flags = PROT_READ | PROT_WRITE | PROT_EXEC;
  // i32 m_flags = MAP_ANON | MAP_SHARED;
  // result = mmap(NULL, size, p_flags, m_flags, -1, 0);
  #endif
  
  result = malloc(size);

  return result;
}

void os_free(void *ptr, u64 size)
{
  #ifdef BACKEND_WINDOWS
  // VirtualFree();
  #endif

  #ifdef BACKEND_UNIX
  munmap(ptr, size);
  #endif

  free(ptr);
}

// @File /////////////////////////////////////////////////////////////////////////////////

OS_Handle os_open(String path, OS_Flag flag)
{
  OS_Handle result = {0};

  #ifdef BACKEND_WINDOWS
  u32 access;
  switch (flag)
  {
    case OS_FLAG_R: access = GENERIC_READ; break;
    case OS_FLAG_W: access = GENERIC_WRITE; break;
    case OS_FLAG_RW: access = GENERIC_READ | GENERIC_WRITE; break;
    default: return (OS_Handle) {0}; break;
  }

  result.data = CreateFileA(path.str, 
                            access, 
                            FILE_SHARE_READ | FILE_SHARE_WRITE, 
                            NULL, OPEN_EXISTING, 
                            FILE_ATTRIBUTE_NORMAL, 
                            NULL);
  #endif

  return result;
}

void os_close(OS_Handle handle)
{
  #ifdef BACKEND_WINDOWS
  CloseHandle(handle.data);
  #endif
}

String os_read(OS_Handle handle, u64 count, u64 pos, Arena *arena)
{
  String result = {0};
  result.str = arena_push(arena, count);

  #ifdef BACKEND_WINDOWS
  SetFilePointer(handle.data, pos, NULL, FILE_BEGIN);
  ReadFile(handle.data, result.str, count, (unsigned long *) &result.len, NULL);
  #endif

  return result;
}

void os_write(OS_Handle handle, String buf)
{
  #ifdef BACKEND_WINDOWS
  WriteFile(handle.data, buf.str, buf.len, NULL, NULL);
  #endif
}

void os_set_file_pos(OS_Handle handle, u64 pos)
{
  #ifdef BACKEND_WINDOWS
  SetFilePointer(handle.data, pos, NULL, FILE_BEGIN);
  #endif

  #ifdef BACKEND_UNIX
  handle = (OS_Handle) {0};
  pos = 0;
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
