#include "globals.h"
#include "util.h"

f64 time_in_seconds()
{
    return (f64) SDL_GetTicks64() * 0.001f;
}
