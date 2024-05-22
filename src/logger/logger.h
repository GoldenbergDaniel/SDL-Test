#pragma once

#include <stdio.h>

#include "../base/base_inc.h"
#include "../base/base_arena.h"
#include "../base/base_string.h"

typedef struct Logger Logger;
struct Logger
{
  Arena *arena;
  FILE *file;
};

Logger create_logger(String path_to_log);
void logger_write(const char *str);
