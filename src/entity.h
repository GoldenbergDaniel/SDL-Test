#pragma once

#include "base/base_inc.h"

#include "gfx/draw.h"
#include "phys/physics.h"

#define GRAVITY 48.0f // 0.8 p/f^2

#define PLAYER_SPEED 600.0f // 10 p/f
#define PLAYER_JUMP_VEL 12.0f // 12.0 p/f
#define PLAYER_ACC 3.0f // 0.05 p/f^2
#define PLAYER_FRIC 8.0f // 0.13 p/f^2

#define TIMER_COMBAT 0
#define TIMER_HEALTH 1
#define TIMER_KILL 2

typedef struct Game Game;
typedef struct Entity Entity;

typedef enum EntityType
{
  EntityType_Nil,
  EntityType_General,
  EntityType_Player,
  EntityType_ZombieWalker,
  EntityType_ZombieBird,
  EntityType_Equipped,
  EntityType_Laser,
  EntityType_Wall,
  EntityType_Debug,
} EntityType;

typedef enum EntityProp
{
  EntityProp_Controlled = 1 << 0,
  EntityProp_Autonomous = 1 << 1,
  EntityProp_Moves = 1 << 2,
  EntityProp_Combatant = 1 << 3,
  EntityProp_Killable = 1 << 4,
  EntityProp_Collides = 1 << 5,
  EntityProp_Rendered = 1 << 6,
  EntityProp_Equipped = 1 << 7,
} EntityProp;

typedef enum MoveType
{
  MoveType_None,
  MoveType_Walking,
  MoveType_Projectile,
  MoveType_Flying,
} MoveType;

typedef enum CombatType
{
  CombatType_None,
  CombatType_Melee,
  CombatType_Ranged,
} CombatType;

typedef enum DrawType
{
  DrawType_None,
  DrawType_Triangle,
  DrawType_Rectangle,
  DrawType_Sprite,
} DrawType;

typedef struct Timer Timer;
struct Timer
{
  f64 max_duration;
  f64 curr_duration;
  bool should_tick;
  bool ticking;
  bool timeout;
  bool should_loop;
  bool start_at_zero;
};

typedef struct EntityRef EntityRef;
struct EntityRef
{
  Entity *ptr;
  u64 id;
  u64 gen;
};

typedef struct Entity Entity;
struct Entity
{
  Entity *next;
  Entity *next_free;

  EntityRef parent;
  EntityRef *children;
  u16 child_capacity;
  u16 child_count;

  // General
  u64 id;
  u64 gen;
  EntityType type;
  MoveType move_type;
  CombatType combat_type;
  b64 props;
  bool active;
  bool visible;

  // Transform
  Vec2F origin;
  Vec2F pos;
  f32 rot;
  Vec2F scale;
  Mat3x3F xform;
  Mat3x3F model_mat;
  Vec2F input_dir;

  // Physics
  // Vec2F dir;
  Vec2F vel;
  Vec2F new_vel;
  f32 speed;
  bool grounded;
  
  // Drawing
  DrawType draw_type;
  Vec4F color;
  D_TextureID texture;
  Vec2F size;
  bool flip_x;
  bool flip_y;
  u16 z_index;

  // Collision
  P_Collider col;
  u8 col_layer;
  b8 col_mask;

  // Targetting
  bool has_target;
  Vec2F target_pos;
  f32 target_angle;
  u16 view_dist;

  char curr_health;

  Timer timers[3];
};

typedef struct EntityList EntityList;
struct EntityList
{
  Entity *head;
  Entity *tail;
  Entity *first_free;
  u16 count;
};

// @InitEntity =================================================================================

void init_entity(Entity *en, EntityType type);
void clear_entity(Entity *entity);

// @UpdateEntity ===============================================================================

void update_entity_collider(Entity *entity);
void update_entity_xform(Game *game, Entity *entity);

void update_controlled_entity_movement(Game *game, Entity *entity);
void update_autonomous_entity_movement(Game *game, Entity *entity);
void update_entity_projectile_movement(Game *game, Entity *entity);
void update_controlled_entity_combat(Game *game, Entity *entity);
void update_targetting_entity_combat(Game *game, Entity *entity);
void update_equipped_entity(Game *game, Entity *entity);

// @SpawnEntity ================================================================================

typedef struct SpawnEntityParams SpawnEntityParams;
struct SpawnEntityParams
{
  b64 props;
  Vec2F pos;
  Vec4F color;
};

#define spawn_entity(game, type, ...) \
  _spawn_entity(game, type, (SpawnEntityParams) \
                {.pos=v2f(0, 0), .color=D_WHITE, __VA_ARGS__ })

Entity *_spawn_entity(Game *game, EntityType type, SpawnEntityParams params);

// @KillEntity =================================================================================

typedef struct KillEntityParams KillEntityParams;
struct KillEntityParams
{
  Entity *entity;
  u64 id;
};

#define kill_entity(game, ...) \
  _kill_entity(game, (KillEntityParams) \
                {.entity=NULL, .id=0, __VA_ARGS__ })

void _kill_entity(Game *game, KillEntityParams params);

// @OtherEntity ================================================================================

Vec2F pos_from_entity(Entity *entity);
f32 rot_from_entity(Entity *entity);
Vec2F scale_from_entity(Entity *entity);
Vec2F size_from_entity(Entity *entity);

void set_entity_target(Entity *entity, EntityRef target);

bool is_entity_valid(Entity *entity);
void resolve_entity_collision(Entity *a, Entity *b);
void wrap_entity_at_edges(Entity *entity);

// @EntityRef ==================================================================================

EntityRef ref_from_entity(Entity *entity);
Entity *entity_from_ref(EntityRef ref);

// @EntityList =================================================================================

Entity *alloc_entity(Game *game);
void free_entity(Game *game, Entity *entity);
Entity *get_entity_of_id(Game *game, u64 id);
Entity *get_first_entity_of_type(Game *game, EntityType type);

// @EntityTree =================================================================================

void set_entity_parent(Entity *entity, Entity *parent);
void assign_entity_child(Entity *entity, Entity *child);
void remove_entity_child(Entity *entity, u64 id);
Entity *get_entity_child_at_index(Entity *en, u16 index);
Entity *get_entity_child_of_id(Entity *entity, u64 id);
Entity *get_entity_child_of_type(Entity *entity, EntityType type);
