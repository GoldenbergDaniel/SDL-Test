#pragma once

#include "entity.h"
#include "draw.h"

typedef struct Prefabs Prefabs;
struct Prefabs
{
  struct
  {
    TextureID player_idle;
    TextureID player_walk_0;
    TextureID player_walk_1;
    TextureID player_walk_2;
    TextureID player_walk_3;
    TextureID player_walk_4;
    TextureID player_walk_5;
    TextureID player_jump;

    TextureID walker_idle;
    TextureID walker_walk_0;
    TextureID walker_walk_1;
    TextureID walker_walk_2;
    TextureID walker_walk_3;
    TextureID walker_walk_4;
    TextureID walker_walk_5;

    TextureID pistol;
    TextureID bullet;
  } texture;

  struct
  {
    AnimationDesc player_idle;
    AnimationDesc player_walk;
  } animation;

  struct
  {
    ParticleDesc smoke;
    ParticleDesc blood;
    ParticleDesc death;
    ParticleDesc debug;
  } particle;
};

void init_prefabs(Prefabs *prefabs);
