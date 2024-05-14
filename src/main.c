#include <stdio.h>
#include <stdlib.h>

#include "base/base_common.h"

#define SOKOL_NO_ENTRY
#include "sokol/sokol_app.h"
#include "sokol/sokol_time.h"

#include "input/input.h"
#include "draw.h"
#include "game.h"
#include "global.h"

#if defined(_WIN32)
#include "glad/glad.h"
#endif

Game GAME;
Global *GLOBAL;
PrefabStore *PREFABS;

void init(void);
void event(const sapp_event*);
void frame(void);

i32 main(void)
{
  sapp_run(&(sapp_desc) {
    .window_title = "Undead West",
    .width = WIDTH,
    .height = HEIGHT,
    .init_cb = init,
    .frame_cb = frame,
    .event_cb = event,
  });

  return 0;
}

void init(void)
{
  GAME.perm_arena = arena_create(MiB(16));
  GAME.entity_arena = arena_create(MiB(32));
  GAME.frame_arena = arena_create(MiB(16));
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

  GLOBAL = arena_alloc(&GAME.perm_arena, sizeof (Global));
  GLOBAL->resources = load_resources(&GAME.perm_arena, res_path);
  GLOBAL->renderer = r_create_renderer(40000, &GAME.batch_arena);

  PREFABS = arena_alloc(&GAME.perm_arena, sizeof (PrefabStore));
  init_particle_prefabs(PREFABS);

  GAME.input = &GLOBAL->input;
  init_game(&GAME);
}

void event(const sapp_event *event)
{
  handle_input_event(event, &GAME.should_quit);
}

void frame(void)
{
  GAME.t = (f64) stm_sec(stm_since(0));
  GAME.dt = sapp_frame_duration();

  update_game(&GAME);
  render_game(&GAME);

  if (game_should_quit(&GAME))
  {
    sapp_quit();
  }
}
