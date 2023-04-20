#pragma once

#include "stdbool.h"
#include "SDL2/SDL.h"

#define TARGET_FPS 60

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 450

#define WINDOW_FLAGS SDL_WINDOW_RESIZABLE
#define RENDERER_FLAGS SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC

#define VIEWPORT_SCALE 4
#define CAM_OFFSET_X 0
#define CAM_OFFSET_Y -50

#define COLOR_BLACK (SDL_Color) {19, 19, 19, 255}
#define COLOR_WHITE (SDL_Color) {236, 236, 236, 255}
#define COLOR_RED (SDL_Color) {229, 78, 48, 255}
#define COLOR_GREEN (SDL_Color) {100, 160, 60, 255}
#define COLOR_BLUE (SDL_Color) {55, 107, 186, 255}

#define GRAVITY 27.0f

#define MILLISECONDS 1000.0f
#define TIME_STEP (f32) 1 / TARGET_FPS * MILLISECONDS

typedef unsigned char u8;
typedef short unsigned int u16;
typedef unsigned int u32;
typedef long unsigned int u64;
typedef char i8;
typedef short int i16;
typedef int i32;
typedef long int i64;
typedef float f32;
typedef long f64;
