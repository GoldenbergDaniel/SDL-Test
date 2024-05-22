#include "logger.h"
#include "stb/stb_sprintf.h"

Logger _LOGGER;

void logger_init(String path_to_log, Arena *arena)
{
  FILE *file;

  if (str_equals(path_to_log, str("stdout")))
  {
    file = stdout;
  }
  else
  {
    String path = str_nullify(path_to_log, arena);
    fopen(path.str, "wb");
  }

  _LOGGER.arena = arena;
  _LOGGER.file = file;
}

void logger_write(const char *str)
{
  fprintf(_LOGGER.file, "%s", str);
}
