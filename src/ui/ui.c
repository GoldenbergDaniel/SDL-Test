#include "../base/base_common.h"
#include "../base/base_arena.h"
#include "ui.h"

UI_WidgetStore WIDGET_STORE;

void ui_init_widgetstore(u64 count, Arena *arena)
{
  WIDGET_STORE.data = arena_push(arena, sizeof (UI_Widget) * count);
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

void ui_text(String text, Vec2F pos, u32 size)
{
  ui_push_widget(&(UI_Widget) {
    .type = UI_WidgetType_Text,
    .pos = pos,
    .string = text,
    .size = size,
    .scale = v2f(1.0f, 1.0f),
  });
}
