#pragma once

#include <assert.h>
#include <stdint.h>
#include <float.h>

typedef uint8_t bool;
typedef uint8_t b8;
typedef uint16_t b16;
typedef uint32_t b32;
typedef uint64_t b64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef float f32;
typedef double f64;

#define auto __auto_type

#if defined(__APPLE__)
#define RO_SECTION_NAME "__DATA, __const"
#elif defined(__linux__)
#define RO_SECTION_NAME ".rodata"
#endif

#if defined(__GNUC__)
#define thread_local __thread
#define read_only __attribute__((section(RO_SECTION_NAME)))
#elif defined(_MSC_VER)
#define thread_local __declspec(thread)
#endif

#define SCOPE(title)

#define TRUE 1
#define FALSE 0

#ifndef NULL
#define NULL ((void *) 0)
#endif

#define zero(x, T) x = ((T) {0})
#define arr_len(arr) (sizeof (arr) / sizeof (arr[0]))
