#pragma once

#include "stdbool.h"
#include "SDL2/SDL.h"

// Window
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 450
#define WINDOW_FLAGS SDL_WINDOW_RESIZABLE

// Renderer
#define RENDERER_FLAGS SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC

// Color
#define COLOR_BLACK (SDL_Color) {19, 19, 19, 255}
#define COLOR_WHITE (SDL_Color) {236, 236, 236, 255}
#define COLOR_RED (SDL_Color) {229, 78, 48, 255}
#define COLOR_GREEN (SDL_Color) {100, 160, 60, 255}
#define COLOR_BLUE (SDL_Color) {55, 107, 186, 255}

// Time
#define MILLISECONDS 1000.0f
#define TIME_STEP (f32) 1 / 60 * MILLISECONDS

// Types
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
