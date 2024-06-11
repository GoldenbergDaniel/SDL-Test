#pragma once

#include "base_os.h"
#include "base_string.h"

typedef struct Logger Logger;
struct Logger
{
  Arena *arena;
  OS_Handle output;
};

void logger_init(String path, Arena *arena);
void logger_debug(String str, ...);
void logger_error(String str, ...);
void logger_output(String str, ...);
