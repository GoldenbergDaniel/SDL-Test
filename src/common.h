#pragma once

typedef unsigned char bool;
typedef unsigned char b8;
typedef unsigned short b16;
typedef unsigned int b32;
typedef unsigned long b64;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;
typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long i64;
typedef float f32;
typedef double f64;

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 450

#define TRUE 1
#define FALSE 0

#define arr_len(arr) (sizeof (arr) / sizeof (arr[0]))

#define log_msg(msg) (printf("%s\n", msg))
#define log_i8(caption, data) (printf("%s%c\n", caption, data))
#define log_i32(caption, data) (printf("%s%i\n", caption, data))
#define log_u32(caption, data) (printf("%s%u\n", caption, data))
#define log_f32(caption, data) (printf("%s%f\n", caption, data))
