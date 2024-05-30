#pragma once

#include "../base/base_string.h"
#include "../base/base_math.h"

typedef struct UI_Glyph UI_Glyph;
struct UI_Glyph
{
  Vec2I coords;
  Vec2F dim;
  Vec2F offset;
};

typedef enum UI_WidgetType
{
  UI_WidgetType_Nil,
  UI_WidgetType_Text,
} UI_WidgetType;

typedef struct UI_Widget UI_Widget;
struct UI_Widget
{
  UI_WidgetType type;

  Vec2F pos;
  Vec2F dim;
  Vec2F scale;

  String text;
  u32 text_size;
  u32 text_spacing;
  u32 space_width;
};

typedef struct UI_WidgetStore UI_WidgetStore;
struct UI_WidgetStore
{
  UI_Widget *data;
  u64 capacity;
  u64 count;
};

#define ui_glyph(x, y, w, h, dx, dy) {{x, y}, {w, h}, {dx, dy}}

void ui_init_widgetstore(u64 count, Arena *arena);
UI_WidgetStore *ui_get_widgetstore(void);
void ui_clear_widgetstore(void);
void ui_push_widget(UI_Widget *widget);

void ui_text(String text, Vec2F pos, f32 size, f32 width);
