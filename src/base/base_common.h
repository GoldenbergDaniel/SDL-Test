#pragma once

#include <assert.h>
#include <stdint.h>

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

#if defined(_WIN32)
#define PLATFORM_WINDOWS
#elif defined(__APPLE__)
#define PLATFORM_MACOS
#define PLATFORM_UNIX
#elif defined (__linux__)
#define PLATFORM_LINUX
#define PLATFORM_UNIX
#endif

#if defined(__GNUC__)
#define COMPILER_CLANG
#endif
#if defined(_MSC_VER)
#define COMPILER_MSVC
#endif

#if defined(PLATFORM_APPLE)
#define _RO_SECTION_NAME "__DATA, __const"
#elif defined(PLATFORM_LINUX)
#define _RO_SECTION_NAME ".rodata"
#endif

#if defined(COMPILER_CLANG)
#define thread_local __thread
#define read_only __attribute__((section(_RO_SECTION_NAME)))
#elif defined(COMPILER_MSVC)
#define thread_local __declspec(thread)
#endif

#define FALSE 0
#define TRUE 1

#ifndef NULL
#define NULL ((void *) 0)
#endif

#define zero(x, T) x = ((T) {0})

// @Math /////////////////////////////////////////////////////////////////////////////////

#define PI 3.14159265359f
#define RADIANS (PI / 180.0f)
#define DEGREES (180.0f / PI)

#define absv(a) (((a) < 0) ? (-(a)) : (a))
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define clamp_top(a, b) (min(a, b))
#define clamp_bot(a, b) (max(a, b))
#define clamp(x, a, b) (((x) < (a)) ? (a) : \
                        ((x) > (b)) ? (b) : (x))
#define round(a) ((i32) (a) + 0.5f))
#define to_zero(a, tol) ((absv(a) - tol) <= 0.0f ? 0.0f : (a))
#define dir(a) (((a) != 0) ? ((a) / absv(a)) : 0)

typedef union Vec2F Vec2F;
union Vec2F
{
  struct
  {
    union
    {
      f32 x;
      f32 width;
    };

    union
    {
      f32 y;
      f32 height;
    };
  };

  f32 e[2];
};

typedef union Vec2I Vec2I;
union Vec2I
{
  struct
  {
    union
    {
      i32 x;
      i32 width;
    };

    union
    {
      i32 y;
      i32 height;
    };
  };

  i32 e[2];
};

typedef union Vec3F Vec3F;
union Vec3F
{
  struct
  {
    f32 x;
    f32 y;
    f32 z;
  };

  f32 e[3];
};

typedef union Vec4F Vec4F;
union Vec4F
{
  struct
  {
    union { f32 x; f32 r; };
    union { f32 y; f32 g; };
    union { f32 z; f32 b; };
    union { f32 w; f32 a; };
  };

  f32 e[4];
};

typedef union Mat2x2F Mat2x2F;
union Mat2x2F
{
  f32 e[2][2];
  Vec2F cols[2];
};

typedef union Mat3x3F Mat3x3F;
union Mat3x3F
{
  f32 e[3][3];
  Vec3F cols[3];
};

typedef union Mat4x4F Mat4x4F;
union Mat4x4F
{
  f32 e[4][4];
  Vec4F cols[4];
};
