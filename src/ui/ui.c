#include <stdarg.h>

#include "../base/base_common.h"
#include "../base/base_arena.h"
#include "../vecmath/vecmath.h"
#include "ui.h"

#include "stb/stb_sprintf.h"

#define BUFFER_SIZE 1024

UI_WidgetStore _widget_store;

void ui_init_widgetstore(u64 count, Arena *arena)
{
  _widget_store.arena = create_arena(GiB(1), FALSE);
  _widget_store.data = arena_push(arena, UI_Widget, count);
  _widget_store.capacity = count;
  _widget_store.count = 0;
}

inline
UI_WidgetStore *ui_get_widgetstore(void)
{
  return &_widget_store;
}

inline
void ui_clear_widgetstore(void)
{
  _widget_store.count = 0;
}

void ui_push_widget(UI_Widget *widget)
{
  assert(_widget_store.capacity > _widget_store.count);

  _widget_store.data[_widget_store.count] = *widget;
  _widget_store.count += 1;
}

void ui_text(String text, Vec2F pos, u32 size, u32 width, ...)
{
  va_list vargs;
  va_start(vargs, width);

  char *buf = arena_push(&_widget_store.arena, char, BUFFER_SIZE);
  i32 len = stbsp_vsprintf(buf, text.data, vargs);

  String formatted_text = str_from_cstring(buf, &_widget_store.arena);
  formatted_text.len = len;

  ui_push_widget(&(UI_Widget) {
    .type = UI_WidgetType_Text,
    .pos = pos,
    .dim = v2f(width, 1),
    .scale = v2f(1.0f, 1.0f),
    .text = formatted_text,
    .text_size = size,
    .text_spacing = v2f(1, 1),
    .space_width = 4,
  });

  va_end(vargs);
}
