#pragma once

#include "base/base_inc.h"

#include "render/render.h"
#include "input/input.h"
#include "draw.h"
#include "entity.h"

#define WIDTH 1270
#define HEIGHT 740

#define SPRITE_SCALE ((Vec2F) {6, 6})

typedef struct Global Global;
struct Global
{
  Input input;
  Resources resources;
  R_Renderer renderer;

  bool debug;
};

typedef struct PrefabStore PrefabStore;
struct PrefabStore
{
  ParticleDesc smoke_particles;
  ParticleDesc blood_particles;
  ParticleDesc debug_particles;
};

typedef struct Game Game;

void init_prefabs(PrefabStore *prefabs);
Vec2F screen_to_world(Vec2F pos);
