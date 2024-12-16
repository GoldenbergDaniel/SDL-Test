#pragma once

#include "../base/base_common.h"
#include "../base/base_string.h"

typedef struct UI_Sprite UI_Sprite;
struct UI_Sprite
{
  Vec2I coord;
  Vec2I grid;
};

typedef struct UI_Glyph UI_Glyph;
struct UI_Glyph
{
  Vec2I coords;
  Vec2F dim;
  Vec2F off;
};

typedef enum UI_WidgetType
{
  UI_WidgetType_Nil,
  UI_WidgetType_Rect,
  UI_WidgetType_Text,
} UI_WidgetType;

typedef struct UI_Widget UI_Widget;
struct UI_Widget
{
  UI_WidgetType type;
  Vec2F pos;
  Vec2F dim;
  Vec2F scale;
  UI_Sprite sprite;
  String text;
  u32 text_size;
  Vec2F text_spacing;
  u32 space_width;
};

typedef struct UI_WidgetStore UI_WidgetStore;
struct UI_WidgetStore
{
  Arena arena;
  UI_Widget *data;
  u64 capacity;
  u64 count;
};

#define ui_glyph(x, y, w, h, dx, dy) {{x, y}, {w, h}, {dx, dy}}

void ui_init_widgetstore(u64 count, Arena *arena);
UI_WidgetStore *ui_get_widgetstore(void);
void ui_clear_widgetstore(void);
void ui_push_widget(UI_Widget *widget);

void ui_rect(Vec2F pos, Vec2F dim, UI_Sprite sprite);
void ui_text(String text, Vec2F pos, u32 size, u32 width, ...);
