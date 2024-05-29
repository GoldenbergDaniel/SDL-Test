#pragma once

#include "../base/base_string.h"
#include "../base/base_math.h"

typedef struct UI_Glyph UI_Glyph;
struct UI_Glyph
{
  Vec2I tex_coord;
  Vec2F dim;
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
  Vec2F scale;
  u32 size;
  String string;
};

typedef struct UI_WidgetStore UI_WidgetStore;
struct UI_WidgetStore
{
  UI_Widget *data;
  u64 capacity;
  u64 count;
};

#define ui_glyph(x, y, w, h) ((UI_Glyph) {(Vec2I) {x, y}, (Vec2F) {w, h}})

void ui_init_widgetstore(u64 count, Arena *arena);
UI_WidgetStore *ui_get_widgetstore(void);
void ui_clear_widgetstore(void);
void ui_push_widget(UI_Widget *widget);

void ui_text(String text, Vec2F pos, u32 size);
