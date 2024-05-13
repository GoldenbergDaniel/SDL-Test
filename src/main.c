#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SOKOL_DLL
#define SOKOL_NO_ENTRY
#include "sokol/sokol_app.h"
#include "sokol/sokol_time.h"

#include "base/base_common.h"
#include "input/input.h"
#include "draw.h"
#include "game.h"
#include "global.h"

#include <sys/param.h>
#include <mach-o/dyld.h>

#define SIMULATION_RATE 60

sapp_desc sokol_entry(Game *game);

Global *GLOBAL;
PrefabStore *PREFABS;

i32 main(void)
{
  Game *game = malloc(sizeof (Game));
  game->perm_arena = arena_create(MiB(16));
  game->frame_arena = arena_create(MiB(16));
  game->batch_arena = arena_create(MiB(16));
  game->entity_arena = arena_create(MiB(32));

  sapp_desc desc = sokol_entry(game);
  sapp_run(&desc);

  return 0;
}

void init(void *data)
{
  Game *gm = (Game *) data;
  srand(time(NULL));
  arena_get_scratch(NULL);

  stm_setup();
  uint64_t start = stm_now();
  gm->t = start;

  clear_frame(V4F_ZERO);

  #if defined(__APPLE__) && defined(RELEASE)
  char buf[MAXPATHLEN];
  u32 size = MAXPATHLEN;
  _NSGetExecutablePath(buf, &size);
  String res_path = (String) {buf, size};
  i64 loc = str_find(res_path, str("undeadwest"), 0, size);
  res_path = str_substr(res_path, 0, loc);
  res_path = str_concat(res_path, str("../Resources/res"), &gm->frame_arena);
  res_path = str_nullify(res_path, &gm->frame_arena);
  #elif defined(__APPLE__) && !defined(RELEASE)
  char buf[MAXPATHLEN];
  u32 size = MAXPATHLEN;
  _NSGetExecutablePath(buf, &size);
  String res_path = (String) {buf, size};
  i64 loc = str_find(res_path, str("undeadwest"), 0, size);
  res_path = str_substr(res_path, 0, loc);
  res_path = str_concat(res_path, str("res"), &gm->frame_arena);
  res_path = str_nullify(res_path, &gm->frame_arena);
  #else
  String res_path = str("res");
  #endif

  GLOBAL = arena_alloc(&gm->perm_arena, sizeof (Global));
  GLOBAL->resources = load_resources(&gm->perm_arena, res_path);
  GLOBAL->renderer = r_create_renderer(40000, &gm->batch_arena);
  
  gm->input = &GLOBAL->input;

  PREFABS = arena_alloc(&gm->perm_arena, sizeof (PrefabStore));
  init_particle_prefabs(PREFABS);

  Game prev_game = {0};
  prev_game.perm_arena = arena_create(KiB(16));
  prev_game.frame_arena = arena_create(MiB(8));
  prev_game.batch_arena = arena_create(MiB(8));
  prev_game.entity_arena = arena_create(MiB(64));

  init_game(gm);
}

void event(const sapp_event *event, void *data)
{
  Game *gm = (Game *) data;
  handle_input_event(event, &gm->should_quit);
}

void frame(void *data)
{
  Game *gm = (Game *) data;
  gm->t = (f64) stm_sec(stm_since(0));
  gm->dt = sapp_frame_duration();

  update_game(gm);
  draw_game(gm, NULL);

  if (game_should_quit(gm))
  {
    sapp_quit();
  }
}

sapp_desc sokol_entry(Game *game)
{
  return (sapp_desc) {
    .width = WIDTH,
    .height = HEIGHT,
    .window_title = "Undead West",
    .swap_interval = 1,
    .init_userdata_cb = init,
    .frame_userdata_cb = frame,
    .event_userdata_cb = event,
    .user_data = game,
  };
}
