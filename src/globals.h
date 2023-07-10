#pragma once

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 450

#define COLOR_BLACK ((SDL_Color) {19, 19, 19, 255})
#define COLOR_WHITE ((SDL_Color) {236, 236, 236, 255})
#define COLOR_RED ((SDL_Color) {249, 58, 38, 255})
#define COLOR_GREEN ((SDL_Color) {100, 160, 60, 255})
#define COLOR_BLUE ((SDL_Color) {55, 107, 186, 255})
#define COLOR_PURPLE ((SDL_Color) {44, 45, 84, 255})

#define TRUE 1
#define FALSE 0

#define AND &
#define OR |
#define XOR ^
#define NOT ~
#define LSHIFT <<
#define RSHIFT >>

#define arr_len(arr) (sizeof (arr) / sizeof (arr[0]))
#define arr_ptr_len(arr) (sizeof (*arr) / sizeof (arr[0]))

#define log(msg) (printf("%s\n", msg))
#define log_char(caption, data) (printf("%s%c\n", caption, data))
#define log_int(caption, data) (printf("%s%i\n", caption, data))
#define log_uint(caption, data) (printf("%s%u\n", caption, data))
#define log_float(caption, data) (printf("%s%f\n", caption, data))

#define func
#define function

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
