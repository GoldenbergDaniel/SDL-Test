#pragma once

#include "base/base_common.h"
#include "base/base_math.h"
#include "util.h"

typedef enum EntityType
{
  EntityType_Ship,
  EntityType_Astreroid,
} EntityType;

typedef enum EntityState
{
  EntityState_Idle,
  EntityState_Move,
  EntityState_Slide,
} EntityState;

typedef enum EntityFlag
{
  EntityFlag_Movable = 1,
  EntityFlag_Killable = 1 << 1,
  EntityFlag_Enemy = 1 << 2,
} EntityFlag;

typedef struct Entity Entity;
struct Entity
{
  // General
  EntityType type;
  EntityState state;
  u64 flags;
  Mat3x3F xform;
  f32 rot;
  Vec2F scale;
  f32 width;
  f32 height;
  Vec4F color;
  bool player : 1;
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
};

#define PLAYER_HEALTH 3
#define PLAYER_SPEED 120.0f
#define PLAYER_ACC 2.5f
#define PLAYER_FRIC 1.5f

Entity create_player_entity(void);
Entity create_enemy_entity(EntityType type);
void update_entity_movement(Entity *entity, f64 dt);
void set_entity_target(Entity *entity, Vec2F target_pos);
void wrap_entity_at_edges(Entity *entity);
void hurt_entity(Entity *entity);
