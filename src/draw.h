#pragma once

#include "base_common.h"
#include "render.h"

// @Draw ====================================================================================

typedef struct D_Stream D_Stream;
struct D_Stream
{
  R_Object vao;
  R_Shader shader;
  Mat3x3F camera;
};

void d_clear(Vec4F color);
void d_draw_rect(D_Stream *stream, Mat3x3F xform, Vec4F color);
