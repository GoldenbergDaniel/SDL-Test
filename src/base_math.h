#pragma once

#include "base_common.h"

#define PI 3.14159265359

#define abs(a) (((a) < 0) ? (-(a)) : (a))
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define clamp_top(a, b) (min(a, b))
#define clamp_bot(a, b) (max(a, b))
#define clamp(x, a, b) (((x) < (a)) ? (a) : \
                        ((x) > (b)) ? (b) : (x))
#define round(a) ((i32) (a) + 0.5f))
#define to_zero(a, tol) ((abs(a) - abs(tol)) <= 0.0f ? 0.0f : (a))
#define dir(a) (((a) != 0) ? ((a) / abs(a)) : 0)

typedef union Vec2F Vec2F;
union Vec2F
{
  struct
  {
    f32 x;
    f32 y;
  };

  f32 elements[2];
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

  f32 elements[3];
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

  f32 elements[4];
};

typedef struct Mat3x3F Mat3x3F;
struct Mat3x3F
{
  f32 elements[3][3];
};

typedef struct Mat4x4F Mat4x4F;
struct Mat4x4F
{
  f32 elements[4][4];
};

// @Scalar ==================================================================================

inline 
f32 lerp_1f(f32 curr, f32 target, f32 rate)
{
  return curr + (target - curr) * rate;
}

// @Vec2F ===================================================================================

#define V2F_ZERO ((Vec2F) {0.0f, 0.0f})

Vec2F v2f(f32 x, f32 y);

Vec2F add_2f(Vec2F a, Vec2F b);
Vec2F sub_2f(Vec2F a, Vec2F b);
Vec2F mul_2f(Vec2F a, Vec2F b);
Vec2F div_2f(Vec2F a, Vec2F b);
f32 dot_2f(Vec2F a, Vec2F b);
f32 cross_2f(Vec2F a, Vec2F b);
Vec2F scale_2f(Vec2F v, f32 scale);

f32 magnitude_2f(Vec2F a);
f32 magnitude_squared_2f(Vec2F a);
f32 distance_2f(Vec2F a, Vec2F b);
f32 distance_squared_2f(Vec2F a, Vec2F b);
Vec2F normalize_2f(Vec2F a);

Vec2F lerp_2f(Vec2F curr, Vec2F target, f32 rate);

// @Vec3F ===================================================================================

#define V3F_ZERO ((Vec3F) {0.0f, 0.0f, 0.0f})

Vec3F v3f(f32 x, f32 y, f32 z);

Vec3F add_3f(Vec3F a, Vec3F b);
Vec3F sub_3f(Vec3F a, Vec3F b);
Vec3F mul_3f(Vec3F a, Vec3F b);
Vec3F div_3f(Vec3F a, Vec3F b);
f32 dot_3f(Vec3F a, Vec3F b);
Vec3F cross_3f(Vec3F a, Vec3F b);
Vec3F scale_3f(Vec3F v, f32 scale);
Vec3F transform_3f(Vec3F v, Mat3x3F m);

f32 magnitude_3f(Vec3F v);
f32 magnitude_squared_3f(Vec3F v);
f32 distance_3f(Vec3F a, Vec3F b);
f32 distance_squared_3f(Vec3F a, Vec3F b);
Vec3F normalize_3f(Vec3F v);

Vec3F lerp_3f(Vec3F curr, Vec3F target, f32 rate);

// @Vec4F ===================================================================================

#define V4F_ZERO ((Vec4F) {0.0f, 0.0f, 0.0f, 0.0f})

// #define COLOR_BLACK ((Vec4F) {19, 19, 19, 255})
// #define COLOR_WHITE ((Vec4F) {236, 236, 236, 255})
// #define COLOR_RED ((Vec4F) {249, 58, 38, 255})
// #define COLOR_GREEN ((Vec4F) {100, 160, 60, 255})
// #define COLOR_BLUE ((Vec4F) {55, 107, 186, 255})
// #define COLOR_PURPLE ((Vec4U) {44, 45, 84, 255})

Vec4F v4f(f32 x, f32 y, f32 z, f32 w);

Vec4F add_4f(Vec4F a, Vec4F b);
Vec4F sub_4f(Vec4F a, Vec4F b);
Vec4F mul_4f(Vec4F a, Vec4F b);
Vec4F div_4f(Vec4F a, Vec4F b);
f32 dot_4f(Vec4F a, Vec4F b);
Vec4F scale_4f(Vec4F v, f32 scale);
Vec4F transform_4f(Vec4F v, Mat4x4F m);

f32 magnitude_4f(Vec4F v);
f32 magnitude_squared_4f(Vec4F v);
f32 distance_4f(Vec4F a, Vec4F b);
f32 distance_squared_4f(Vec4F a, Vec4F b);
Vec4F normalize_4f(Vec4F v);

// @Mat3x3F =================================================================================

Mat3x3F m3x3f(f32 k);

Mat3x3F rows_3x3f(Vec3F v1, Vec3F v2, Vec3F v3);
Mat3x3F cols_3x3f(Vec3F v1, Vec3F v2, Vec3F v3);

Mat3x3F mul_3x3f(Mat3x3F a, Mat3x3F b);
Mat3x3F transpose_3x3f(Mat3x3F m);

Mat3x3F translate_3x3f(f32 x_shift, f32 y_shift);
Mat3x3F rotate_3x3f(f32 angle);
Mat3x3F scale_3x3f(f32 x_scale, f32 y_scale);
Mat3x3F shear_3x3f(f32 x_shear, f32 y_shear);

Mat3x3F orthographic_3x3f(f32 left, f32 right, f32 bot, f32 top);

// @Mat4x4F =================================================================================

Mat4x4F m4x4f(f32 k);

Mat4x4F rows_4x4f(Vec4F v1, Vec4F v2, Vec4F v3, Vec4F v4);
Mat4x4F cols_4x4f(Vec4F v1, Vec4F v2, Vec4F v3, Vec4F v4);
Mat4x4F m4x4f(f32 k);

Mat4x4F mul_4x4f(Mat4x4F a, Mat4x4F b);
Mat4x4F transpose_4x4f(Mat4x4F m);

Mat4x4F translate_4x4f(f32 x_shift, f32 y_shift, f32 z_shift);
Mat4x4F rotate_4x4f(f32 angle, Vec3F axis); // Should use quaternion
Mat4x4F scale_4x4f(f32 x_scale, f32 y_scale, f32 z_scale);
Mat4x4F shear_4x4f(f32 x_shear, f32 y_shear, f32 z_shear);

Mat4x4F orthographic_4x4f(f32 left, f32 right, f32 bot, f32 top);

// @Collision ===============================================================================

Vec2F rect_center(Vec2F pos, f32 w, f32 h);

bool range_intersect(f32 min1, f32 max1, f32 min2, f32 max2);
bool rect_ranges_intersect(Vec2F p1, Vec2F p2, f32 w1, f32 h1, f32 w2, f32 h2);
bool rect_intersect(void);

#ifdef __cplusplus

// @Overloading =============================================================================

Vec2F operator+(Vec2F a, Vec2F b);
Vec3F operator+(Vec3F a, Vec3F b);
Vec4F operator+(Vec4F a, Vec4F b);
Vec2F operator-(Vec2F a, Vec2F b);
Vec3F operator-(Vec3F a, Vec3F b);
Vec4F operator-(Vec4F a, Vec4F b);
f32 operator*(Vec2F a, Vec2F b);
f32 operator*(Vec3F a, Vec3F b);
f32 operator*(Vec4F a, Vec4F b);
Mat3x3F operator*(Mat3x3F a, Mat3x3F b);
Mat4x4F operator*(Mat4x4F a, Mat4x4F b);

#endif
