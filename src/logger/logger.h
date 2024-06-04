#pragma once

#include "../base/base_os.h"
#include "../base/base_arena.h"
#include "../base/base_string.h"

typedef struct Logger Logger;
struct Logger
{
  Arena *arena;
  OS_Handle file;
};

Logger create_logger(String path_to_log);
void logger_write(String s);
