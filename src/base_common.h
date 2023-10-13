#pragma once

#include <assert.h>
#include <stdint.h>

typedef uint8_t bool;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef char byte;
typedef char i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef float f32;
typedef double f64;

#define THREAD_LOCAL __thread

#define ASSERT(exp) assert(exp)
#define ARR_LEN(arr) (sizeof (arr) / sizeof (arr[0]))

#ifndef NULL
#define NULL (void *) 0
#endif

#ifdef __GNUC__
#define typeof(type) __typeof__(type)
#endif

#define VSYNC_AUTO -1
#define VSYNC_OFF 0
#define VSYNC_ON 1

#define W_WIDTH 1200
#define W_HEIGHT 675
#define W_FLAGS SDL_WINDOW_OPENGL
#define W_CENTERED SDL_WINDOWPOS_CENTERED

#define TRUE 1
#define FALSE 0

#define arr_len(arr) (sizeof (arr) / sizeof (arr[0]))

#define print(msg) (printf("%s\n", msg))
