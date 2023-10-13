#pragma once

#include "base_common.h"
#include "base_math.h"
#include "util.h"

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
  Mat3x3F xform;
  f32 rotation;
  Vec2F scale;
  f32 width;
  f32 height;
  Vec4F color;
  bool active : 1;

  // Physics
  Vec2F pos;
  Vec2F dir;
  Vec2F vel;
  f32 speed;
  u8 col_mask;

  // Targetting
  bool has_target : 1;
  Vec2F target_pos;
  f32 target_angle;
  u16 view_dist;

  // Health
  i8 health;
  Timer hurt_cooldown;
};

#define PLAYER_HEALTH 3
#define PLAYER_SPEED 360.0f
#define PLAYER_ACC 10.0f
#define PLAYER_FRIC 1.5f

Entity entity_create(EntityType type);
void entity_start(Entity *entity);
void entity_update(Entity *entity, f64 dt);
void entity_set_target(Entity *entity, Vec2F target_pos);
void entity_deal_damage(Entity *target);
