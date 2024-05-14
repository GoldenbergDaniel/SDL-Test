#define BACKEND_LIBC
// #define BACKEND_UNIX

#ifdef BACKEND_LIBC
#include <stdlib.h>
#endif

#ifdef BACKEND_UNIX
#include <sys/mman.h>
#include <sys/dir.h>
#endif

#ifdef __APPLE__
#include <sys/param.h>
#include <mach-o/dyld.h>
#undef bool
#endif

#include "base_common.h"
#include "base_os.h"

inline
void *os_alloc(u64 size)
{
  #ifdef BACKEND_LIBC
  return malloc(size);
  #endif

  #ifdef BACKEND_UNIX
  i32 p_flags = PROT_READ | PROT_WRITE | PROT_EXEC;
  i32 m_flags = MAP_ANON | MAP_SHARED;
  return mmap(NULL, size, p_flags, m_flags, -1, 0);
  #endif
}

inline
void *os_map_file(i32 file, u64 size, i32 offset)
{
  #ifdef BACKEND_UNIX
  i32 p_flags = PROT_READ | PROT_WRITE | PROT_EXEC;
  i32 m_flags = MAP_SHARED;
  return mmap(NULL, size, p_flags, m_flags, file, offset);
  #endif

  #ifdef BACKEND_LIBC
  return 0;
  #endif
}

inline
i32 os_set_prot(void *ptr, u64 size, i32 flags)
{
  #ifdef BACKEND_LIBC
  return 0;
  #endif

  #ifdef BACKEND_UNIX
  return mprotect(ptr, size, flags);
  #endif
}

inline
void os_free(void *ptr, u64 size)
{
  #ifdef BACKEND_LIBC
  free(ptr);
  #endif

  #ifdef BACKEND_UNIX
  munmap(ptr, size);
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
