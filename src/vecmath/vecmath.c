#include "../base/base_common.h"
#include "vecmath.h"

f32 sinf(f32);
f32 cosf(f32);
f32 tanf(f32);
f32 sqrtf(f32);
f32 powf(f32, f32);
f32 atan2f(f32, f32);

// @Scalar =====================================================================================

inline
f32 sin_1f(f32 angle)
{
  return sinf(angle);
}

inline
f32 cos_1f(f32 angle)
{
  return cosf(angle);
}

inline
f32 tan_1f(f32 angle)
{
  return tanf(angle);
}

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
Vec2I v2i(i32 x, i32 y)
{
  return (Vec2I) {x, y};
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
Vec2F shift_2f(Vec2F v, f32 shift)
{
  return (Vec2F) {v.x + shift, v.y + shift};
}

inline
Vec2F transform_2f(Vec2F v, Mat2x2F m)
{
  Vec2F result = {0};
  result.x += m.e[0][0] * v.e[0];
  result.x += m.e[0][1] * v.e[1];
  result.y += m.e[1][0] * v.e[0];
  result.y += m.e[1][1] * v.e[1];

  return result;
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
Vec2F project_2f(Vec2F a, Vec2F b)
{
  return scale_2f(b, dot_2f(a, b) / dot_2f(b, b));
}

inline
Vec2F lerp_2f(Vec2F curr, Vec2F target, f32 rate)
{
  return add_2f(curr, scale_2f(sub_2f(target, curr), rate));
}

inline
Vec2F normal_2f(Vec2F a, Vec2F b)
{
  return normalize_2f(v2f(-(b.y - a.y), (b.x - a.x)));
}

inline
Vec2F midpoint_2f(Vec2F a, Vec2F b)
{
  return v2f((a.x+b.x)/2.0f, (a.y+b.y)/2.0f);
}

Vec2F intersection_2f(Vec2F a, Vec2F b, Vec2F c, Vec2F d)
{
  Vec2F ab = sub_2f(b, a);
  Vec2F cd = sub_2f(d, c);
  Vec2F ac = sub_2f(c, a);
  f32 t = cross_2f(ac, cd) / cross_2f(ab, cd);

  return add_2f(a, scale_2f(ab, t));
}

inline
f32 atan_2f(Vec2F a)
{
  return atan2f(a.y, a.x);
}

// @Vec3F ======================================================================================

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
  result.x += m.e[0][0] * v.e[0];
  result.x += m.e[0][1] * v.e[1];
  result.x += m.e[0][2] * v.e[2];
  result.y += m.e[1][0] * v.e[0];
  result.y += m.e[1][1] * v.e[1];
  result.y += m.e[1][2] * v.e[2];
  result.z += m.e[2][0] * v.e[0];
  result.z += m.e[2][1] * v.e[1];
  result.z += m.e[2][2] * v.e[2];

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
  return add_3f(curr, scale_3f(sub_3f(target, curr), rate));
}

// @Vec4F ======================================================================================

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
  result.x += m.e[0][0] * v.e[0];
  result.x += m.e[0][1] * v.e[1];
  result.x += m.e[0][2] * v.e[2];
  result.x += m.e[0][3] * v.e[3];
  result.y += m.e[1][0] * v.e[0];
  result.y += m.e[1][1] * v.e[1];
  result.y += m.e[1][2] * v.e[2];
  result.y += m.e[1][3] * v.e[3];
  result.z += m.e[2][0] * v.e[0];
  result.z += m.e[2][1] * v.e[1];
  result.z += m.e[2][2] * v.e[2];
  result.z += m.e[2][3] * v.e[3];
  result.w += m.e[3][0] * v.e[0];
  result.w += m.e[3][1] * v.e[1];
  result.w += m.e[3][2] * v.e[2];
  result.w += m.e[3][3] * v.e[3];

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

inline
Vec4F lerp_4f(Vec4F curr, Vec4F target, f32 rate)
{
  return add_4f(curr, scale_4f(sub_4f(target, curr), rate));
}

// @Mat2x2F =================================================================================

Mat2x2F m2x2f(f32 k)
{
  return (Mat2x2F)
  {
    {
      {k, 0},
      {0, k}
    }
  };
}

Mat2x2F rows_2x2f(Vec2F v1, Vec2F v2)
{
  return (Mat2x2F)
  {
    {
      {v1.x, v1.y},
      {v2.x, v2.y},
    }
  };
}

Mat2x2F cols_2x2f(Vec2F v1, Vec2F v2)
{
  return (Mat2x2F)
  {
    {
      {v1.x, v2.x},
      {v1.y, v2.y},
    }
  };
}

Mat2x2F mul_2x2f(Mat2x2F a, Mat2x2F b)
{
  Mat2x2F result = {0};

  for (u8 r = 0; r < 2; r++)
  {
    for (u8 c = 0; c < 2; c++)
    {
      result.e[r][c] += a.e[r][0] * b.e[0][c];
      result.e[r][c] += a.e[r][1] * b.e[1][c];
    }
  }

  return result;
}

Mat2x2F transpose_2x2f(Mat2x2F m)
{
  Mat2x2F result = m;
  result.e[0][1] = m.e[1][0];
  result.e[1][0] = m.e[0][1];

  return result;
}

Mat2x2F inverse_2x2f(Mat2x2F m)
{
  Mat2x2F result =
  {
    {
      {m.e[1][1], -m.e[0][1]},
      {-m.e[1][0], m.e[0][0]}
    }
  };

  f32 det = (m.e[0][0] * m.e[1][1]) - (m.e[0][0] * m.e[1][1]);
  result.e[0][0] = (1.0f / det) * result.e[0][0];
  result.e[0][1] = (1.0f / det) * result.e[0][1];
  result.e[1][0] = (1.0f / det) * result.e[1][0];
  result.e[1][1] = (1.0f / det) * result.e[1][1];

  return result;
}

// @Mat3x3F ====================================================================================

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
      result.e[r][c] += a.e[r][0] * b.e[0][c];
      result.e[r][c] += a.e[r][1] * b.e[1][c];
      result.e[r][c] += a.e[r][2] * b.e[2][c];
    }
  }

  return result;
}

Mat3x3F transpose_3x3f(Mat3x3F m)
{
  Mat3x3F result = m;
  result.e[0][1] = m.e[1][0];
  result.e[0][2] = m.e[2][0];
  result.e[1][0] = m.e[0][1];
  result.e[1][2] = m.e[2][1];
  result.e[2][0] = m.e[0][2];
  result.e[2][1] = m.e[1][2];

  return result;
}

// TODO: Learn how this works
Mat3x3F invert_3x3f(Mat3x3F m)
{
  Mat3x3F cross;
  cross.cols[0] = cross_3f(m.cols[1], m.cols[2]);
  cross.cols[1] = cross_3f(m.cols[2], m.cols[0]);
  cross.cols[2] = cross_3f(m.cols[0], m.cols[1]);

  f32 inv_det = 1.0f / dot_3f(cross.cols[2], m.cols[2]);

  Mat3x3F result;
  result.cols[0] = scale_3f(cross.cols[0], inv_det);
  result.cols[1] = scale_3f(cross.cols[1], inv_det);
  result.cols[2] = scale_3f(cross.cols[2], inv_det);

  return transpose_3x3f(result);
}

Mat3x3F translate_3x3f(f32 x_shift, f32 y_shift)
{
  Mat3x3F result = m3x3f(1.0f);
  result.e[0][2] = x_shift;
  result.e[1][2] = y_shift;

  return result;
}

// Angle is in radians
Mat3x3F rotate_3x3f(f32 angle)
{
  Mat3x3F result = m3x3f(1.0f);
  result.e[0][0] = cos_1f(angle);
  result.e[0][1] = -sin_1f(angle);
  result.e[1][0] = sin_1f(angle);
  result.e[1][1] = cos_1f(angle);

  return result;
}

Mat3x3F scale_3x3f(f32 x_scale, f32 y_scale)
{
  Mat3x3F result = m3x3f(1.0f);
  result.e[0][0] = x_scale;
  result.e[1][1] = y_scale;

  return result;
}

Mat3x3F shear_3x3f(f32 x_shear, f32 y_shear)
{
  Mat3x3F result = m3x3f(1.0f);
  result.e[0][1] = x_shear;
  result.e[1][0] = y_shear;

  return result;
}

Mat3x3F orthographic_3x3f(f32 left, f32 right, f32 top, f32 bot)
{
  Mat3x3F result = m3x3f(0.0f);
  result.e[0][0] = 2.0f / (right - left);
  result.e[1][1] = 2.0f / (top - bot);
  result.e[0][2] = -(right + left) / (right - left);
  result.e[1][2] = -(top + bot) / (top - bot);
  result.e[2][2] = 1.0f;

  return result;
}

// @Mat4x4F ====================================================================================

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
      result.e[r][c] += a.e[0][c] * b.e[r][0];
      result.e[r][c] += a.e[1][c] * b.e[r][1];
      result.e[r][c] += a.e[2][c] * b.e[r][2];
      result.e[r][c] += a.e[3][c] * b.e[r][3];
    }
  }

  return result;
}

Mat4x4F transpose_4x4f(Mat4x4F m)
{
  Mat4x4F result = m;
  result.e[0][1] = m.e[1][0];
  result.e[0][2] = m.e[2][0];
  result.e[0][3] = m.e[3][0];
  result.e[1][0] = m.e[0][1];
  result.e[1][2] = m.e[2][1];
  result.e[1][3] = m.e[3][1];
  result.e[2][0] = m.e[0][2];
  result.e[2][1] = m.e[1][2];
  result.e[2][3] = m.e[3][2];
  result.e[3][0] = m.e[0][3];
  result.e[3][1] = m.e[1][3];
  result.e[3][2] = m.e[2][3];

  return result;
}

Mat4x4F translate_4x4f(f32 x_shift, f32 y_shift, f32 z_shift)
{
  Mat4x4F result = m4x4f(1.0f);
  result.e[0][3] = x_shift;
  result.e[1][3] = y_shift;
  result.e[2][3] = z_shift;

  return result;
}

Mat4x4F scale_4x4f(f32 x_scale, f32 y_scale, f32 z_scale)
{
  Mat4x4F result = m4x4f(1.0f);
  result.e[0][0] = x_scale;
  result.e[1][1] = y_scale;
  result.e[2][2] = z_scale;

  return result;
}

Mat4x4F orthographic_4x4f(f32 left, f32 right, f32 bot, f32 top)
{
  static f32 near = -1.0f;
  static f32 far = 1.0f;

  Mat4x4F result = {0};
  result.e[0][0] = 2.0f / (right - left);
  result.e[1][1] = 2.0f / (top - bot);
  result.e[2][2] = -2.0f / (far - near);
  result.e[0][3] = -(right + left) / (right - left);
  result.e[1][3] = -(top + bot) / (top - bot);
  result.e[2][3] = -(far + near) / (far - near);
  result.e[3][3] = 1.0f;

  return m4x4f(1);
}

#ifdef __cplusplus

// @Overloading ================================================================================

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
