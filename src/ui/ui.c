#include "../base/base_common.h"
#include "../base/base_arena.h"
#include "../vecmath/vecmath.h"
#include "ui.h"

#include "stb/stb_sprintf.h"

#define BUFFER_SIZE 1024

UI_WidgetStore WIDGET_STORE;

void ui_init_widgetstore(u64 count, Arena *arena)
{
  WIDGET_STORE.data = arena_push(arena, UI_Widget, count);
  WIDGET_STORE.capacity = count;
  WIDGET_STORE.count = 0;
}

inline
UI_WidgetStore *ui_get_widgetstore(void)
{
  return &WIDGET_STORE;
}

inline
void ui_clear_widgetstore(void)
{
  WIDGET_STORE.count = 0;
}

void ui_push_widget(UI_Widget *widget)
{
  assert(WIDGET_STORE.capacity > WIDGET_STORE.count);

  WIDGET_STORE.data[WIDGET_STORE.count] = *widget;
  WIDGET_STORE.count += 1;
}

void ui_text(String text, Vec2F pos, f32 size, f32 width)
{
  ui_push_widget(&(UI_Widget) {
    .type = UI_WidgetType_Text,
    .pos = pos,
    .dim = v2f(width, 1),
    .scale = v2f(1.0f, 1.0f),
    .text = text,
    .text_size = size,
    .text_spacing = v2f(1, 1),
    .space_width = 4,
  });
}

void ui_text_1f(String text, f32 val, Vec2F pos, f32 size, Arena *arena)
{
  char *buf = arena_push(arena, char, BUFFER_SIZE);
  stbsp_snprintf(buf, BUFFER_SIZE, text.data, val);

  String formatted_text = str_from_cstring(buf, arena);
  ui_push_widget(&(UI_Widget) {
    .type = UI_WidgetType_Text,
    .pos = pos,
    .dim = v2f(9999, 1),
    .scale = v2f(1.0f, 1.0f),
    .text = formatted_text,
    .text_size = size,
    .text_spacing = v2f(1, 1),
    .space_width = 4,
  });
}

void ui_text_2f(String text, Vec2F val, Vec2F pos, f32 size, Arena *arena)
{
  char *buf = arena_push(arena, char, BUFFER_SIZE);
  stbsp_snprintf(buf, BUFFER_SIZE, text.data, val.x, val.y);

  String formatted_text = str_from_cstring(buf, arena);
  ui_push_widget(&(UI_Widget) {
    .type = UI_WidgetType_Text,
    .pos = pos,
    .dim = v2f(9999, 1),
    .scale = v2f(1.0f, 1.0f),
    .text = formatted_text,
    .text_size = size,
    .text_spacing = v2f(1, 1),
    .space_width = 4,
  });
}
