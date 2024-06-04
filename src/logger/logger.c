#include "stb/stb_sprintf.h"

#include "../base/base_os.h"
#include "logger.h"

Logger _LOGGER;

void logger_init(String path, Arena *arena)
{
  path = str_nullify(path, arena);
  OS_Handle file = os_open(path, OS_FILE_WRITE);
  arena_pop(arena, path.len);

  _LOGGER.arena = arena;
  _LOGGER.file = file;
}

void logger_write(String s)
{
  // stbsp_snprintf(buf, 255, text.str, val);

  os_write(_LOGGER.file, s);
}
