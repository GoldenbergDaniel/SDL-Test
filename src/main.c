#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef near
#undef far

#include "glad/glad.c"
#endif

#define SOKOL_NO_ENTRY
#include "sokol/sokol_app.h"
#include "sokol/sokol_time.h"
#include "sokol/sokol_log.h"

#include "base/base_os.c"
#include "base/base_arena.c"
#include "base/base_string.c"
#include "base/base_math.c"
#include "base/base_random.c"
#include "render/render.c"
#include "ui/ui.c"
#include "phys/phys.c"
#include "prefabs.c"
#include "draw.c"
#include "input.c"
#include "entity.c"
#include "game.c"

Globals global;
Prefabs prefab;
Game game;

void init(void);
void event(const sapp_event *);
void frame(void);

#if defined(_WIN32) && defined(RELEASE)
i32 CALLBACK WinMain(HINSTANCE _a, HINSTANCE _b, LPSTR _c, i32 _d)
#else
i32 main(void)
#endif
{
  sapp_run(&(sapp_desc) {
    .window_title = "Undead West",
    .width = WIDTH,
    .height = HEIGHT,
    .high_dpi = FALSE,
    .init_cb = init,
    .event_cb = event,
    .frame_cb = frame,
    .logger = {
      .func = slog_func
    },
  });

  return 0;
}

void init(void)
{
  stm_setup();
  srand((u32) stm_now());
  get_scratch_arena(NULL);

  #if defined(_WIN32)
  gladLoadGL();
  #endif

  #if defined(__APPLE__) && defined(RELEASE)
  String res_path = os_path_to_executable(str("undeadwest"));
  res_path = str_concat(res_path, str("../Resources/res"), &GAME.frame_arena);
  res_path = str_nullify(res_path, &GAME.frame_arena);
  #else
  String res_path = str("res");
  #endif

  global.perm_arena = create_arena(MiB(64));
  global.window.width = sapp_width();
  global.window.height = sapp_height();
  global.viewport = v4f(0, 0, sapp_width(), sapp_height());

  global.frame.current_time = stm_sec(stm_since(0));
  global.frame.accumulator = TIME_STEP;

  global.resources = load_resources(&global.perm_arena, res_path);
  global.renderer = r_create_renderer(40000, WIDTH, HEIGHT, &global.perm_arena);

  prefab = create_prefabs();

  game.entity_arena = create_arena(MiB(16));
  game.frame_arena = create_arena(MiB(16));
  game.draw_arena = create_arena(MiB(16));

  game.dt = TIME_STEP;
  game.input = &global.input;
  init_game(&game);
}

void event(const sapp_event *event)
{
  handle_input_event(event);
}

void frame(void)
{
  // Update viewport ----------------
  if (global.window.width != sapp_width() || global.window.height != sapp_height())
  {
    Vec4F viewport;
    f32 ratio = (f32) sapp_width() / sapp_height();
    if (ratio >= WIDTH / HEIGHT)
    {
      f32 img_width = sapp_width() / (ratio / (WIDTH / HEIGHT));
      viewport = v4f((sapp_width() - img_width) / 2.0f, 0.0f, img_width, sapp_height());
    }
    else
    {
      f32 img_height = sapp_height() * (ratio / (WIDTH / HEIGHT));
      viewport = v4f(0.0f, (sapp_height() - img_height) / 2.0f, sapp_width(), img_height);
    }

    r_set_viewport(viewport.x, viewport.y, viewport.z, viewport.w);
    global.viewport = viewport;
  }
  
  global.window.width = sapp_width();
  global.window.height = sapp_height();
  
  f64 new_time = stm_sec(stm_since(0));
  f64 frame_time = new_time - global.frame.current_time;
  global.frame.current_time = new_time;
  global.frame.accumulator += frame_time;

  // Simulation loop ----------------
  while (global.frame.accumulator >= TIME_STEP)
  {
    if (is_key_released(KEY_ENTER))
    {
      sapp_toggle_fullscreen();
    }

    game.t = stm_sec(stm_since(0));
    update_game(&game);
    arena_clear(&game.frame_arena);

    remember_last_keys();

    global.frame.elapsed_time += TIME_STEP;
    global.frame.accumulator -= TIME_STEP;
  }

  render_game(&game);
  arena_clear(&game.draw_arena);

  if (game_should_quit(&game))
  {
    sapp_quit();
  }
}
