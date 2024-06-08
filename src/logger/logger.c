#include <stdarg.h>
#include "stb/stb_sprintf.h"

#include <windows.h>
#include "../base/base.h"
#include "../os/os.h"
#include "logger.h"

Logger _logger;

void logger_init(String path, Arena *arena)
{
  _logger.arena = arena;

  if (!str_equals(path, str("")))
  {
    _logger.output = os_open(path, OS_FILE_WRITE);
  }
}

void logger_debug(String str, ...)
{
  assert(_logger.arena->size > 0);

  va_list vargs;
  va_start(vargs, str);
  
  u64 size = _logger.arena->size;
  String text = alloc_str(size - ARENA_ALIGN_SIZE, _logger.arena);
  text.len = stbsp_vsnprintf(text.data, size, str.data, vargs);
  
  os_write(os_handle_to_stdout(), text);
  os_windows_output_debug(text.data);

  arena_clear(_logger.arena);
  va_end(vargs);
}

void logger_error(String str, ...)
{
  assert(_logger.arena->size > 0);

  va_list vargs;
  va_start(vargs, str);
  
  u64 size = _logger.arena->size;
  String text = alloc_str(size - ARENA_ALIGN_SIZE, _logger.arena);
  text.len = stbsp_vsnprintf(text.data, size, str.data, vargs);
  
  os_write(os_handle_to_stderr(), text);
  os_windows_output_debug(text.data);

  arena_clear(_logger.arena);
  va_end(vargs);
}

void logger_output(String str, ...)
{
  assert(_logger.output.id != 0);

  va_list vargs;
  va_start(vargs, str);
  
  u64 size = _logger.arena->size;
  String text = alloc_str(size - ARENA_ALIGN_SIZE, _logger.arena);
  text.len = stbsp_vsnprintf(text.data, size, str.data, vargs);
  
  os_write(_logger.output, text);

  arena_clear(_logger.arena);
  va_end(vargs);
}
