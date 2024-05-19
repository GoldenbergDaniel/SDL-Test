#pragma once

#include "base/base_common.h"
#include "render/render.h"
#include "input.h"
#include "draw.h"
#include "entity.h"

#ifdef _WIN32
#define TIME_STEP (1.0f / 120)
#else
#define TIME_STEP (1.f / 60)
#endif

#define WIDTH 960
#define HEIGHT 540
#define SPRITE_SCALE ((Vec2F) {5, 5})

typedef struct Frame Frame;
struct Frame
{
  f64 current_time;
  f64 elapsed_time;
  f64 accumulator;
};

typedef struct Global Global;
struct Global
{
  Input input;
  Frame frame;
  Resources resources;
  R_Renderer renderer;
  Vec2I window_dim;
  bool debug;
};

typedef struct PrefabStore PrefabStore;
struct PrefabStore
{
  struct
  {
    ParticleDesc smoke;
    ParticleDesc blood;
    ParticleDesc death;
    ParticleDesc debug;
  } particle;
};

void init_entity_prefabs(PrefabStore *prefabs);
void init_particle_prefabs(PrefabStore *prefabs);

i32 get_width(void);
i32 get_height(void);
