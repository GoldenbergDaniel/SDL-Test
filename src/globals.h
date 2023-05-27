#pragma once

#include "SDL2/SDL.h"

#define WINDOW_WIDTH (unsigned short) 800
#define WINDOW_HEIGHT (unsigned short) 450
#define TARGET_FPS (unsigned short) 60

#define WINDOW_FLAGS SDL_WINDOW_METAL
#define RENDERER_FLAGS SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC

#define COLOR_BLACK (SDL_Color) {19, 19, 19, 255}
#define COLOR_WHITE (SDL_Color) {236, 236, 236, 255}
#define COLOR_RED (SDL_Color) {249, 58, 38, 255}
#define COLOR_GREEN (SDL_Color) {100, 160, 60, 255}
#define COLOR_BLUE (SDL_Color) {55, 107, 186, 255}

#define TRUE (unsigned char) 1
#define FALSE (unsigned char) 0

typedef unsigned char bool;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;
typedef char i8;
typedef short i16;
typedef int i32;
typedef long i64;
typedef float f32;
typedef double f64;
