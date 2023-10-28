#pragma once

#include "base/base_common.h"
#include "base/base_arena.h"
#include "base/base_math.h"

typedef struct Game Game;

typedef struct Entity Entity;
typedef struct EntityRef EntityRef;
typedef struct EntityList EntityList;
typedef struct Timer Timer;

typedef enum EntityType
{
  EntityType_Nil,
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

struct Timer
{
  f64 start_duration;
  f64 curr_duration;
  bool should_tick;
  bool ticking;
  bool timeout;
  bool loop;
};

struct EntityRef
{
  Entity *entity;
  u64 id;
};

struct Entity
{
  Entity *next;
  Entity *next_free;

  // General
  u64 id;
  EntityType type;
  b64 props;
  f32 rot;
  Vec2F scale;
  f32 width;
  f32 height;
  Mat3x3F xform;
  Vec4F color;
  u16 z_index; // NOTE: probably just use inserion sort for now?
  bool active;
  bool visible;

  // Physics
  Vec2F pos;
  Vec2F dir;
  Vec2F vel;
  f32 speed;
  i8 health;

  // Targetting
  bool has_target;
  Vec2F target_pos;
  f32 target_angle;
  u16 view_dist;

  // Timers
  Timer timers[3];
};

struct EntityList
{
  Entity *head;
  Entity *tail;
  Entity *first_free;
  u16 count;
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
#define TIMER_KILL 2

// @Entity =====================================================================================

void init_player_entity(Entity *entity);
void init_enemy_entity(Entity *entity);
void init_laser_entity(Entity *entity);

void clear_entity(Entity *entity);

void update_entity_xform(Game *game, Entity *entity);
void update_controlled_entity_movement(Game *game, Entity *entity);
void update_targetting_entity_movement(Game *game, Entity *entity);
void update_projectile_entity_movement(Game *game, Entity *entity);
void update_controlled_entity_combat(Game *game, Entity *entity);
void update_targetting_entity_combat(Game *game, Entity *entity);

void set_entity_target(Entity *entity, EntityRef target);
void wrap_entity_at_edges(Entity *entity);
void damage_entity(Entity *entity, i8 damage);

// @EntityRef ==================================================================================

EntityRef ref_from_entity(Entity *entity);
Entity *entity_from_ref(EntityRef ref);

// @EntityList =================================================================================

Entity *alloc_entity(Game *game);
void free_entity(Game *game, Entity *entity);
Entity *get_entity_of_id(Game *game, u64 id);
Entity *get_nearest_entity_of_type(Game *game, Vec2F pos, EntityType type);
