#pragma once

#include "base_common.h"
#include "base_math.h"
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
  Vec4F color;
  f32 width;
  f32 height;

  // Physics
  Vec2F pos;
  Vec2F vel;
  Vec2F dir;
  f32 speed;
  u8 col_mask;

  // Targetting
  bool has_target;
  Vec2F target_pos;
  f32 target_angle;
  u16 view_dist;

  // Health
  i8 health;
  bool is_active;
  Timer hurt_cooldown;
};

#define PLAYER_HEALTH 3
#define PLAYER_SPEED 1200.0f
#define PLAYER_ACC 10.0f
#define PLAYER_FRIC 1.5f

Entity entity_create(EntityType type);
void entity_start(Entity *entity);
void entity_update(Entity *entity, f64 dt);
void entity_set_target(Entity *entity, Vec2F target_pos);
void entity_deal_damage(Entity *target);
