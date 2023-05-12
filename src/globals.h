#pragma once

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 450

#define WINDOW_FLAGS 0
#define RENDERER_FLAGS SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
#define IMAGE_INIT_FLAGS IMG_INIT_PNG | IMG_INIT_JPG

#define COLOR_BLACK (SDL_Color) {19, 19, 19, 255}
#define COLOR_WHITE (SDL_Color) {236, 236, 236, 255}
#define COLOR_RED (SDL_Color) {249, 58, 38, 255}
#define COLOR_GREEN (SDL_Color) {100, 160, 60, 255}
#define COLOR_BLUE (SDL_Color) {55, 107, 186, 255}

#define TIMESTEP 1.0f / 60.0f

#define TRUE 1
#define FALSE 0

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
