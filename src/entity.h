#pragma once

#include "common.h"
#include "util.h"
#include "component.h"

typedef enum EntityType
{
  EntityType_Player,
  EntityType_Enemy,
} EntityType;

typedef enum EntityState
{
  EntityState_Idle,
  EntityState_Move,
  EntityState_Slide,
} EntityState;

typedef struct Entity Entity;
struct Entity
{
  // General
  EntityType type;
  EntityState move_state;
  Vec4U8 color;
  f32 width;
  f32 height;

  // Physics
  Vec2F32 pos;
  Vec2F32 vel;
  Vec2F32 dir;
  f32 speed;
  u8 col_mask;

  // Targetting
  bool has_target;
  Vec2F32 target_pos;
  f32 target_angle;
  u16 view_dist;

  // Health
  i8 health;
  bool is_active;
  Timer hurt_cooldown;
};

// #define PLAYER_HEALTH 3
// #define PLAYER_SPEED 300.0f
// #define PLAYER_ACC 30.0f

Entity entity_create(EntityType type);
void entity_start(Entity *entity);
void entity_update(Entity *entity, f64 dt);
void entity_set_target(Entity *entity, Vec2F32 target_pos);
void entity_deal_damage(Entity *target);
