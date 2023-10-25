#pragma once

#include "base/base_common.h"
#include "base/base_math.h"
#include "base/base_arena.h"
#include "component.h"

typedef struct Game Game;

typedef enum EntityType
{
  EntityType_Player,
  EntityType_EnemyShip,
  EntityType_Laser,
} EntityType;

typedef enum EntityProp
{
  EntityProp_Controlled = 1 << 0,
  EntityProp_Targetting = 1 << 1,
  EntityProp_Movable = 1 << 2,
  EntityProp_Attacker = 1 << 3,
  EntityProp_Killable = 1 << 4,
  EntityProp_Projectile = 1 << 5,
} EntityProp;

typedef struct Entity Entity;
struct Entity
{
  Entity *next;

  // General
  EntityType type;
  b64 props;
  f32 rot;
  Vec2F scale;
  f32 width;
  f32 height;
  Mat3x3F xform;
  Vec4F color;
  bool active;

  // Physics
  Vec2F pos;
  Vec2F dir;
  Vec2F vel;
  f32 speed;

  // Targetting
  bool has_target;
  Vec2F target_pos;
  f32 target_angle;
  u16 view_dist;

  // Timers
  Timer timers[2];

  // Health
  i8 health;
};

#define PLAYER_HEALTH 3
#define PLAYER_SPEED 120.0f
#define PLAYER_ACC 2.5f
#define PLAYER_FRIC 1.5f

#define PLAYER_PROPS EntityProp_Controlled | EntityProp_Movable | EntityProp_Attacker
#define ENEMY_PROPS EntityProp_Targetting | EntityProp_Movable | EntityProp_Attacker
#define LASER_PROPS EntityProp_Movable | EntityProp_Projectile

#define TIMER_COMBAT 0
#define TIMER_HEALTH 1

Entity *create_player_entity(Game *game);
Entity *create_enemy_entity(Game *game);
Entity *create_laser_entity(Game *game);

void update_entity_xform(Game *game, Entity *entity);
void update_controlled_entity_movement(Game *game, Entity *entity);
void update_targetting_entity_movement(Game *game, Entity *entity);
void update_projectile_entity_movement(Game *game, Entity *entity);
void update_controlled_entity_combat(Game *game, Entity *entity);
void update_targetting_entity_combat(Game *game, Entity *entity);

void set_entity_target(Game *game, Entity *entity, Entity *target);
void wrap_entity_at_edges(Entity *entity);
void hurt_entity(Game *game, Entity *entity);
