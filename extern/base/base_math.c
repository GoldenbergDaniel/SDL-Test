#include <math.h>

#include "base_common.h"
#include "base_math.h"

// @Scalar =====================================================================================

inline 
f32 lerp_1f(f32 curr, f32 target, f32 rate)
{
  return curr + (target - curr) * rate;
}

// @Vec2F ======================================================================================

inline
Vec2F v2f(f32 x, f32 y)
{
  return (Vec2F) {x, y};
}

inline
Vec2F add_2f(Vec2F a, Vec2F b)
{
  return (Vec2F) {a.x + b.x, a.y + b.y};
}

inline
Vec2F sub_2f(Vec2F a, Vec2F b)
{
  return (Vec2F) {a.x - b.x, a.y - b.y};
}

inline
Vec2F mul_2f(Vec2F a, Vec2F b)
{
  return (Vec2F) {a.x * b.x, a.y * b.y};
}

inline
Vec2F div_2f(Vec2F a, Vec2F b)
{
  return (Vec2F) {a.x / b.x, a.y / b.y};
}

inline
f32 dot_2f(Vec2F a, Vec2F b)
{
  return (a.x * b.x) + (a.y * b.y);
}

inline
f32 cross_2f(Vec2F a, Vec2F b)
{
  f32 result;
  result = (a.x * b.y) -(a.y * b.x);

  return result;
}

inline
Vec2F scale_2f(Vec2F v, f32 scale)
{
  return (Vec2F) {v.x * scale, v.y * scale};
}

inline
f32 magnitude_2f(Vec2F v)
{
  return sqrtf(powf(v.x, 2.0f) + powf(v.y, 2.0f));
}

inline
f32 magnitude_squared_2f(Vec2F v)
{
  return powf(v.x, 2.0f) + powf(v.y, 2.0f);
}

inline
f32 distance_2f(Vec2F a, Vec2F b)
{
  Vec2F v = sub_2f(b, a);
  return sqrtf(powf(v.x, 2.0f) + powf(v.y, 2.0f));
}

inline
f32 distance_squared_2f(Vec2F a, Vec2F b)
{
  Vec2F v = sub_2f(b, a);
  return powf(v.x, 2.0f) + powf(v.y, 2.0f);
}

inline
Vec2F normalize_2f(Vec2F v)
{
  return scale_2f(v, 1.0f / magnitude_2f(v));
}

inline
Vec2F lerp_2f(Vec2F curr, Vec2F target, f32 rate)
{
  return scale_2f(sub_2f(target, curr), rate);
}

// @Vec3F ===================================================================================

inline
Vec3F v3f(f32 x, f32 y, f32 z)
{
  return (Vec3F) {x, y, z};
}

inline
Vec3F add_3f(Vec3F a, Vec3F b)
{
  return (Vec3F) {a.x + b.x, a.y + b.y, a.z + b.z};
}

inline
Vec3F sub_3f(Vec3F a, Vec3F b)
{
  return (Vec3F) {a.x - b.x, a.y - b.y, a.z - b.z};
}

inline
Vec3F mul_3f(Vec3F a, Vec3F b)
{
  return (Vec3F) {a.x * b.x, a.y * b.y, a.z * b.z};
}

inline
Vec3F div_3f(Vec3F a, Vec3F b)
{
  return (Vec3F) {a.x / b.x, a.y / b.y, a.z / b.z};
}

inline
f32 dot_3f(Vec3F a, Vec3F b)
{
  return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

inline
Vec3F cross_3f(Vec3F a, Vec3F b)
{
  Vec3F result;
  result.x = (a.y * b.z - a.z * b.y);
  result.y = -(a.x * b.z - a.z * b.x);
  result.z = (a.x * b.y - a.y * b.x);

  return result;
}

inline
Vec3F scale_3f(Vec3F v, f32 scale)
{
  return (Vec3F) {v.x * scale, v.y * scale, v.z * scale};
}

Vec3F transform_3f(Vec3F v, Mat3x3F m)
{
  Vec3F result = {0};
  for (u8 c = 0; c < 3; c++)
  {
    result.x += m.elements[0][c] * v.elements[c];
    result.y += m.elements[1][c] * v.elements[c];
    result.z += m.elements[2][c] * v.elements[c];
  }

  return result;
}

inline
f32 magnitude_3f(Vec3F v)
{
  return sqrtf(powf(v.x, 2.0f) + powf(v.y, 2.0f) + powf(v.z, 2.0f));
}

inline
f32 magnitude_squared_3f(Vec3F v)
{
  return powf(v.x, 2.0f) + powf(v.y, 2.0f) + powf(v.z, 2.0f);
}

inline
f32 distance_3f(Vec3F a, Vec3F b)
{
  Vec3F v = sub_3f(b, a);
  return sqrtf(powf(v.x, 2.0f) + powf(v.y, 2.0f) + powf(v.z, 2.0f));
}

inline
f32 distance_squared_3f(Vec3F a, Vec3F b)
{
  Vec3F v = sub_3f(b, a);
  return powf(v.x, 2.0f) + powf(v.y, 2.0f) + powf(v.z, 2.0f);
}

inline
Vec3F normalize_3f(Vec3F v)
{
  return scale_3f(v, 1.0f / magnitude_3f(v));
}

inline
Vec3F lerp_3f(Vec3F curr, Vec3F target, f32 rate)
{
  return scale_3f(sub_3f(target, curr), rate);
}

// @Vec4F ===================================================================================

inline
Vec4F v4f(f32 x, f32 y, f32 z, f32 w)
{
  return (Vec4F) {x, y, z, w};
}

inline
Vec4F add_4f(Vec4F a, Vec4F b)
{
  return (Vec4F) {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

inline
Vec4F sub_4f(Vec4F a, Vec4F b)
{
  return (Vec4F) {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

inline
Vec4F mul_4f(Vec4F a, Vec4F b)
{
  return (Vec4F) {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
}

inline
Vec4F div_4f(Vec4F a, Vec4F b)
{
  return (Vec4F) {a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w};
}

inline
f32 dot_4f(Vec4F a, Vec4F b)
{
  return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}

inline
Vec4F scale_4f(Vec4F v, f32 scale)
{
  return (Vec4F) {v.x * scale, v.y * scale, v.z * scale, v.w * scale};
}

Vec4F transform_4f(Vec4F v, Mat4x4F m)
{
  Vec4F result = {0};

  for (u8 c = 0; c < 4; c++)
  {
    result.x += m.elements[0][c] * v.elements[c];
    result.y += m.elements[1][c] * v.elements[c];
    result.z += m.elements[2][c] * v.elements[c];
    result.w += m.elements[3][c] * v.elements[c];
  }

  return result;
}

inline
f32 magnitude_4f(Vec4F v)
{
  return sqrtf(powf(v.x, 2.0f) + powf(v.y, 2.0f) + powf(v.z, 2.0f) + powf(v.z, 2.0f));
}

inline
f32 magnitude_squared_4f(Vec4F v)
{
  return powf(v.x, 2.0f) + powf(v.y, 2.0f) + powf(v.z, 2.0f) + powf(v.z, 2.0f);
}

inline
f32 distance_4f(Vec4F a, Vec4F b)
{
  Vec4F v = sub_4f(b, a);
  return sqrtf(powf(v.x, 2.0f) + powf(v.y, 2.0f) + powf(v.z, 2.0f) + powf(v.w, 2.0f));
}

inline
f32 distance_squared_4f(Vec4F a, Vec4F b)
{
  Vec4F v = sub_4f(b, a);
  return powf(v.x, 2.0f) + powf(v.y, 2.0f) + powf(v.z, 2.0f) + powf(v.w, 2.0f);
}

inline
Vec4F normalize_4f(Vec4F v)
{
  return scale_4f(v, 1.0f / magnitude_4f(v));
}

// @Mat3x3F =================================================================================

inline
Mat3x3F m3x3f(f32 k)
{
  return (Mat3x3F)
  {
    {
      {k, 0, 0},
      {0, k, 0},
      {0, 0, k}
    }
  };
}

inline
Mat3x3F rows_3x3f(Vec3F v1, Vec3F v2, Vec3F v3)
{
  return (Mat3x3F)
  {
    {
      {v1.x, v1.y, v1.z},
      {v2.x, v2.y, v2.z},
      {v3.x, v3.y, v3.z}
    }
  };
}

inline
Mat3x3F cols_3x3f(Vec3F v1, Vec3F v2, Vec3F v3)
{
  return (Mat3x3F)
  {
    {
      {v1.x, v2.x, v3.x},
      {v1.y, v2.y, v3.y},
      {v1.z, v2.z, v3.z}
    }
  };
}

Mat3x3F mul_3x3f(Mat3x3F a, Mat3x3F b)
{
  Mat3x3F result = {0};

  for (u8 r = 0; r < 3; r++)
  {
    for (u8 c = 0; c < 3; c++)
    {
      result.elements[r][c] += a.elements[r][0] * b.elements[0][c];
      result.elements[r][c] += a.elements[r][1] * b.elements[1][c];
      result.elements[r][c] += a.elements[r][2] * b.elements[2][c];
    }
  }

  return result;
}

Mat3x3F transpose_3x3f(Mat3x3F m)
{
  Mat3x3F result = m;
  result.elements[0][1] = m.elements[1][0];
  result.elements[0][2] = m.elements[2][0];
  result.elements[1][0] = m.elements[0][1];
  result.elements[1][2] = m.elements[2][1];
  result.elements[2][0] = m.elements[0][2];
  result.elements[2][1] = m.elements[1][2];

  return result;
}

Mat3x3F translate_3x3f(f32 x_shift, f32 y_shift)
{
  Mat3x3F result = m3x3f(1.0f);
  result.elements[0][2] = x_shift;
  result.elements[1][2] = y_shift;

  return result;
}

Mat3x3F rotate_3x3f(f32 angle)
{
  Mat3x3F result = m3x3f(1.0f);
  result.elements[0][0] = cos(angle);
  result.elements[0][1] = -sin(angle);
  result.elements[1][0] = sin(angle);
  result.elements[1][1] = cos(angle);

  return result;
}

Mat3x3F scale_3x3f(f32 x_scale, f32 y_scale)
{
  Mat3x3F result = m3x3f(1.0f);
  result.elements[0][0] = x_scale;
  result.elements[1][1] = y_scale;

  return result;
}

Mat3x3F shear_3x3f(f32 x_shear, f32 y_shear)
{
  Mat3x3F result = m3x3f(1.0f);
  result.elements[0][1] = x_shear;
  result.elements[1][0] = y_shear;

  return result;
}

Mat3x3F orthographic_3x3f(f32 left, f32 right, f32 bot, f32 top)
{
  Mat3x3F result = m3x3f(0.0f);
  result.elements[0][0] = 2.0f / (right - left);
  result.elements[1][1] = 2.0f / (top - bot);
  result.elements[0][2] = -(right + left) / (right - left);
  result.elements[1][2] = -(top + bot) / (top - bot);
  result.elements[2][2] = 1.0f;

  return result;
}

// @Mat4x4F =================================================================================

inline
Mat4x4F m4x4f(f32 k)
{
  return (Mat4x4F)
  {
    {
      {k, 0, 0, 0},
      {0, k, 0, 0},
      {0, 0, k, 0},
      {0, 0, 0, k}
    }
  };
}

inline
Mat4x4F rows_4x4f(Vec4F v1, Vec4F v2, Vec4F v3, Vec4F v4)
{
  return (Mat4x4F)
  {
    {
      {v1.x, v1.y, v1.z, v1.w},
      {v2.x, v2.y, v2.z, v2.w},
      {v3.x, v3.y, v3.z, v3.w},
      {v4.x, v4.y, v4.z, v4.w}
    }
  };
}

inline
Mat4x4F cols_4x4f(Vec4F v1, Vec4F v2, Vec4F v3, Vec4F v4)
{
  return (Mat4x4F)
  {
    {
      {v1.x, v2.x, v3.x, v4.x},
      {v1.y, v2.y, v3.y, v4.y},
      {v1.z, v2.z, v3.z, v4.z},
      {v1.w, v2.w, v3.w, v4.w}
    }
  };
}

Mat4x4F mul_4x4f(Mat4x4F a, Mat4x4F b)
{
  Mat4x4F result = {0};

  for (u8 r = 0; r < 4; r++)
  {
    for (u8 c = 0; c < 4; c++)
    {
      result.elements[r][c] += a.elements[0][c] * b.elements[r][0];
      result.elements[r][c] += a.elements[1][c] * b.elements[r][1];
      result.elements[r][c] += a.elements[2][c] * b.elements[r][2];
      result.elements[r][c] += a.elements[3][c] * b.elements[r][3];
    }
  }

  return result;
}

Mat4x4F transpose_4x4f(Mat4x4F m)
{
  Mat4x4F result = m;
  result.elements[0][1] = m.elements[1][0];
  result.elements[0][2] = m.elements[2][0];
  result.elements[0][3] = m.elements[3][0];
  result.elements[1][0] = m.elements[0][1];
  result.elements[1][2] = m.elements[2][1];
  result.elements[1][3] = m.elements[3][1];
  result.elements[2][0] = m.elements[0][2];
  result.elements[2][1] = m.elements[1][2];
  result.elements[2][3] = m.elements[3][2];
  result.elements[3][0] = m.elements[0][3];
  result.elements[3][1] = m.elements[1][3];
  result.elements[3][2] = m.elements[2][3];

  return result;
}

Mat4x4F translate_4x4f(f32 x_shift, f32 y_shift, f32 z_shift)
{
  Mat4x4F result = m4x4f(1.0f);
  result.elements[0][3] = x_shift;
  result.elements[1][3] = y_shift;
  result.elements[2][3] = z_shift;

  return result;
}

Mat4x4F scale_4x4f(f32 x_scale, f32 y_scale, f32 z_scale)
{
  Mat4x4F result = m4x4f(1.0f);
  result.elements[0][0] = x_scale;
  result.elements[1][1] = y_scale;
  result.elements[2][2] = z_scale;

  return result;
}

Mat4x4F orthographic_4x4f(f32 left, f32 right, f32 bot, f32 top)
{
  const f32 near = -1.0f;
  const f32 far = 1.0f;

  Mat4x4F result = {0};
  result.elements[0][0] = 2.0f / (right - left);
  result.elements[1][1] = 2.0f / (top - bot);
  result.elements[2][2] = -2.0f / (far - near);
  result.elements[0][3] = -(right + left) / (right - left);
  result.elements[1][3] = -(top + bot) / (top - bot);
  result.elements[2][3] = -(far + near) / (far - near);
  result.elements[3][3] = 1.0f;

  return result;
}

// @Collision ===============================================================================

inline
Vec2F rect_center(Vec2F pos, f32 w, f32 h)
{
  return (Vec2F)
  {
    pos.x + (w / 2.0f),
    pos.y + (h / 2.0f)
  };
}

inline
bool range_intersect(f32 min1, f32 max1, f32 min2, f32 max2)
{
  return (max1 >= min2) && (max2 >= min1);
}

bool rect_ranges_intersect(Vec2F p1, Vec2F p2, f32 w1, f32 h1, f32 w2, f32 h2)
{
  return range_intersect(p1.x, p1.x+w1, p2.x, p2.x+w2) &&
         range_intersect(p1.y, p1.y+h1, p2.y, p2.y+h2);
}


#ifdef __cplusplus

// @Overloading =============================================================================

Vec2F operator+(Vec2F a, Vec2F b)
{
  return add_2f(a, b);
}

Vec3F operator+(Vec3F a, Vec3F b)
{
  return add_3f(a, b);
}

Vec4F operator+(Vec4F a, Vec4F b)
{
  return add_4f(a, b);
}

Vec2F operator-(Vec2F a, Vec2F b)
{
  return sub_2f(a, b);
}

Vec3F operator-(Vec3F a, Vec3F b)
{
  return sub_3f(a, b);
}

Vec4F operator-(Vec4F a, Vec4F b)
{
  return sub_4f(a, b);
}

f32 operator*(Vec2F a, Vec2F b)
{
  return dot_2f(a, b);
}

f32 operator*(Vec3F a, Vec3F b)
{
  return dot_3f(a, b);
}

f32 operator*(Vec4F a, Vec4F b)
{
  return dot_4f(a, b);
}

Mat3x3F operator*(Mat3x3F a, Mat3x3F b)
{
  return mul_3x3f(a, b);
}

Mat4x4F operator*(Mat4x4F a, Mat4x4F b)
{
  return mul_4x4f(a, b);
}

#endif
