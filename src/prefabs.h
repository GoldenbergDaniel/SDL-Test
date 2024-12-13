#pragma once

#include "draw.h"
#include "entity.h"
#include "game.h"

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

    TextureID chicken_idle;
    TextureID chicken_lay_0;
    TextureID chicken_lay_1;

    TextureID revolver;
    TextureID pistol;
    TextureID rifle;
    TextureID shotgun;
    TextureID smg;

    TextureID muzzle_flash;
    TextureID bullet;
    TextureID coin;
    TextureID soul;

    TextureID egg_0;
    TextureID egg_1;
    TextureID egg_2;
  } texture;

  struct
  {
    AnimationDesc player_idle;
    AnimationDesc player_walk;
    AnimationDesc player_jump;
    
    AnimationDesc walker_idle;
    AnimationDesc walker_walk;

    AnimationDesc chicken_idle;
    AnimationDesc chicken_lay;
  } animation;

  WaveDesc wave[TOTAL_WAVE_COUNT];
  ParticleDesc particle[ParticleKind_COUNT];
  ZombieDesc zombie[ZombieKind_COUNT];
  WeaponDesc weapon[WeaponKind_COUNT];
  CollectableDesc collectable[CollectableKind_COUNT];
  
};

Prefabs create_prefabs(void);
