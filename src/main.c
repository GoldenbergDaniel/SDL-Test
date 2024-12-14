#if defined(_WIN32)
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
  #undef near
  #undef far
#endif

#if !defined(__APPLE__)
  #include "glad/glad.c"
#endif

#include <stdlib.h>

#include "base/base_common.h"
#include "base/base_os.c"
#include "base/base_arena.c"
#include "base/base_string.c"
#include "base/base_random.c"
#include "base/base_logger.c"
#include "render/render.c"
#include "vecmath/vecmath.c"
#include "ui/ui.c"
#include "physics/physics.c"
#include "prefabs.c"
#include "draw.c"
#include "input.c"
#include "entity.c"
#include "game.c"

#define SOKOL_IMPL
#define SOKOL_GLCORE
#define SOKOL_NO_ENTRY
#include "sokol/sokol_app.h"
#include "sokol/sokol_time.h"

#ifdef DEBUG
  #include "sokol/sokol_log.h"
#endif

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb/stb_image.h"

#define STB_SPRINTF_IMPLEMENTATION
#include "stb/stb_sprintf.h"

Globals global;
Prefabs prefab;
Game game;

void init(void);
void event(const sapp_event *);
void frame(void);

#if defined(_WIN32) && !defined(DEBUG)
i32 WINAPI WinMain(HINSTANCE _a, HINSTANCE _b, LPSTR _c, i32 _d)
#else
i32 main(void)
#endif
{
  Arena logger_arena = create_arena(MiB(64), TRUE);
  init_logger(str(""), &logger_arena);

  sapp_run(&(sapp_desc) {
    .window_title = "Undead West",
    .width = WIDTH,
    .height = HEIGHT,
    .high_dpi = FALSE,
    .init_cb = init,
    .event_cb = event,
    .frame_cb = frame,
    #ifdef DEBUG
    .logger = {
      .func = slog_func
    },
    #endif
  });

  return 0;
}

void init(void)
{
  init_scratch_arenas();

  global.perm_arena = create_arena(GiB(1), TRUE);

  game.entity_arena = create_arena(GiB(1), FALSE);
  game.frame_arena = create_arena(GiB(1), TRUE);
  game.draw_arena = create_arena(MiB(16), FALSE);
  
  stm_setup();
  srand((u32) stm_now());
  get_scratch_arena(NULL);

#if defined(PLATFORM_LINUX) || defined(PLATFORM_WINDOWS)
  gladLoadGL();
#endif

#if defined(PLATFORM_MACOS) && !defined(DEBUG)
  String res_path = os_path_to_executable(str("undeadwest"));
  res_path = str_concat(res_path, str("../Resources/res"), &game.frame_arena);
#else
  String res_path = str("res");
#endif

  global.window.width = sapp_width();
  global.window.height = sapp_height();
  global.viewport = v4f(0, 0, sapp_width(), sapp_height());

  global.frame.current_time = stm_sec(stm_since(0));
  global.frame.accumulator = TIME_STEP;

  global.resources = load_resources(&global.perm_arena, res_path);
  global.renderer = r_create_renderer(40000, WIDTH, HEIGHT, &global.perm_arena);

  prefab = create_prefabs();

  game.dt = TIME_STEP;
  init_game();
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
    f32 ratio = sapp_widthf() / sapp_heightf();
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
    if (is_key_released(Key_Enter))
    {
      sapp_toggle_fullscreen();
    }

    f64 time_start = stm_ns(stm_since(0));
    game.t = stm_sec(stm_since(0));
    update_game();
    f64 time_end = stm_ns(stm_since(0));

    String duration_str = format_duration(time_end - time_start, &game.frame_arena);
    duration_str = str_concat(str("update: "), duration_str, &game.frame_arena);
    duration_str = str_concat(duration_str, str("\n"), &game.frame_arena);
    // logger_debug(duration_str, &game.frame_arena);
    game.update_time = time_end - time_start;

    remember_last_keys();

    global.frame.elapsed_time += TIME_STEP;
    global.frame.accumulator -= TIME_STEP;
  }

  u64 time_start = stm_ns(stm_since(0));
  render_game();
  u64 time_end = stm_ns(stm_since(0));

  String duration_str = format_duration(time_end - time_start, &game.frame_arena);
  duration_str = str_concat(str("render: "), duration_str, &game.frame_arena);
  duration_str = str_concat(duration_str, str("\n"), &game.frame_arena);
  // logger_debug(duration_str, &game.frame_arena);
  game.render_time = time_end - time_start;

  if (game_should_quit())
  {
    sapp_quit();
  }
}
