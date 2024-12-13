#include <stdarg.h>
#include "stb/stb_sprintf.h"

#include "base_logger.h"

Logger _logger;

void init_logger(String path, Arena *arena)
{
  _logger.arena = arena;

  if (!str_equals(path, str("")))
  {
    _logger.output = os_open_file(path, OS_FILE_WRITE);
  }
}

void logger_debug(String str, ...)
{
  assert(_logger.arena->size > 0);

  va_list vargs;
  va_start(vargs, str);
  
  u64 size = KiB(16);
  String text = alloc_str(size, _logger.arena);
  text.len = stbsp_vsnprintf(text.data, size, str.data, vargs);
  
  os_write_file(os_handle_to_stdout(), text);
#ifdef PLATFORM_WINDOWS
  os_windows_output_debug(text.data);
#endif

  arena_clear(_logger.arena);
  va_end(vargs);
}

void logger_error(String str, ...)
{
  assert(_logger.arena->size > 0);

  va_list vargs;
  va_start(vargs, str);
  
  u64 size = KiB(16);
  String text = alloc_str(size, _logger.arena);
  text.len = stbsp_vsnprintf(text.data, size, str.data, vargs);
  
  os_write_file(os_handle_to_stderr(), text);
  #ifdef PLATFORM_WINDOWS
  os_windows_output_debug(text.data);
  #endif

  arena_clear(_logger.arena);
  va_end(vargs);
}

void logger_output(String str, ...)
{
  assert(_logger.output.id != 0);

  va_list vargs;
  va_start(vargs, str);
  
  u64 size = MiB(1);
  String text = alloc_str(size - 8, _logger.arena);
  text.len = stbsp_vsnprintf(text.data, size, str.data, vargs);
  
  os_write_file(_logger.output, text);

  arena_clear(_logger.arena);
  va_end(vargs);
}
