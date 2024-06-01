#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32)
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "glad/glad.h"
#endif

#define SOKOL_NO_ENTRY
#include "sokol/sokol_app.h"
#include "sokol/sokol_time.h"
#include "sokol/sokol_log.h"

#include "base/base_common.h"
#include "render/render.h"
#include "input.h"
#include "draw.h"
#include "game.h"
#include "globals.h"
#include "prefabs.h"

Game GAME;
Globals *GLOBAL;
Prefabs *PREFAB;

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
  GAME.perm_arena = arena_create(MiB(16));
  GAME.entity_arena = arena_create(MiB(32));
  GAME.frame_arena = arena_create(MiB(16));
  GAME.draw_arena = arena_create(MiB(16));
  GAME.batch_arena = arena_create(MiB(16));
  
  stm_setup();
  srand((u32) stm_now());
  arena_get_scratch(NULL);

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

  GLOBAL = arena_push(&GAME.perm_arena, sizeof (Globals));

  GLOBAL->window.width = sapp_width();
  GLOBAL->window.height = sapp_height();
  GLOBAL->viewport = v4f(0, 0, sapp_width(), sapp_height());

  GLOBAL->frame.current_time = stm_sec(stm_since(0));
  GLOBAL->frame.accumulator = TIME_STEP;

  GLOBAL->resources = load_resources(&GAME.perm_arena, res_path);
  GLOBAL->renderer = r_create_renderer(40000, &GAME.batch_arena);

  PREFAB = create_prefabs(&GAME.perm_arena);

  GAME.dt = TIME_STEP;
  GAME.input = &GLOBAL->input;
  init_game(&GAME);
}

void event(const sapp_event *event)
{
  handle_input_event(event);
}

void frame(void)
{
  // Update viewport ----------------
  if (GLOBAL->window.width != sapp_width() || GLOBAL->window.height != sapp_height())
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
    GLOBAL->viewport = viewport;
  }
  
  GLOBAL->window.width = sapp_width();
  GLOBAL->window.height = sapp_height();
  
  f64 new_time = stm_sec(stm_since(0));
  f64 frame_time = new_time - GLOBAL->frame.current_time;
  GLOBAL->frame.current_time = new_time;
  GLOBAL->frame.accumulator += frame_time;

  // Simulation loop ----------------
  while (GLOBAL->frame.accumulator >= TIME_STEP)
  {
    if (is_key_released(KEY_ENTER))
    {
      sapp_toggle_fullscreen();
    }

    GAME.t = stm_sec(stm_since(0));
    update_game(&GAME);
    handle_game_events(&GAME);
    arena_clear(&GAME.frame_arena);

    remember_last_keys();

    GLOBAL->frame.elapsed_time += TIME_STEP;
    GLOBAL->frame.accumulator -= TIME_STEP;
  }

  render_game(&GAME);
  arena_clear(&GAME.draw_arena);
  arena_clear(&GAME.batch_arena);

  if (game_should_quit(&GAME))
  {
    sapp_quit();
  }
}
