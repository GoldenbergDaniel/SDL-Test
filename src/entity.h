#pragma once

#include "base/base_inc.h"
#include "phys/phys.h"
#include "draw.h"

#define GRAVITY 3600.0f

#define PLAYER_SPEED 600.0f
#define PLAYER_JUMP_VEL 900.0f
#define PLAYER_ACC 3.0f
#define PLAYER_FRIC 8.0f

#define PROJ_SPEED 1000.0f

#define MAX_ENTITY_CHILDREN 16

#define TIMER_Combat 0
#define TIMER_HEALTH 1
#define TIMER_KILL 2
#define NUM_TIMERS 3

#define BULLET_KILL_TIME 5.0f
#define PLAYER_ATTACK_COOLDOWN 0.5f
#define ENEMY_ATTACK_COOLDOWN 1.0f

// @Special //////////////////////////////////////////////////////////////////////////////

enum
{
  SP_Player,
  SP_Gun,
};

typedef enum ColliderID
{
  Collider_Body,
  Collider_Head,
  Collider_Hit,

  _Collider_Count,
} ColliderID;

typedef struct Game Game;
typedef struct Entity Entity;

// @Timer ////////////////////////////////////////////////////////////////////////////////

typedef struct Timer Timer;
struct Timer
{
  f32 duration;
  f64 end_time;
  bool is_ticking;
};

// @Particle /////////////////////////////////////////////////////////////////////////////

typedef enum ParticleEmmissionType
{
  ParticleEmmissionType_Burst,
  ParticleEmmissionType_Linear,
} ParticleEmmissionType;

typedef enum ParticleProp
{
  ParticleProp_AffectedByGravity = 1 << 0,
  ParticleProp_CollidesWithGround = 1 << 1,
  ParticleProp_VariateColor = 1 << 2,
  ParticleProp_ScaleOverTime = 1 << 3,
  ParticleProp_SpeedOverTime = 1 << 4,
  ParticleProp_RotateOverTime = 1 << 5,
} ParticleProp;

typedef struct Particle Particle;
struct Particle
{
  Vec4F color;
  Vec2F pos;
  Vec2F scale;
  f32 dir;
  f32 rot;
  Vec2F vel;
  f32 speed;
  bool is_grounded;
};

typedef struct ParticleDesc ParticleDesc;
struct ParticleDesc
{
  ParticleEmmissionType emmission_type;
  b8 props;
  u32 count;
  f32 duration;
  f32 spread;
  Vec4F color_primary;
  Vec4F color_secondary;
  Vec2F scale;
  Vec2F scale_delta;
  f32 speed;
  f32 speed_delta;
  f32 rot_delta;
  Vec2F vel;
};

// @Entity ///////////////////////////////////////////////////////////////////////////////

typedef enum EntityType
{
  EntityType_Nil,
  EntityType_Debug,
  EntityType_Player,
  EntityType_ZombieWalker,
  EntityType_ZombieBird,
  EntityType_Equipped,
  EntityType_Bullet,
  EntityType_Collider,
  EntityType_ParticleGroup,
} EntityType;

typedef enum EntityProp
{
  EntityProp_Renders = 1 << 0,
  EntityProp_Collides = 1 << 1,
  EntityProp_Controlled = 1 << 2,
  EntityProp_Moves = 1 << 3,
  EntityProp_Killable = 1 << 4,
  EntityProp_Equipped = 1 << 5,
  EntityProp_WrapsAtEdges = 1 << 6,
  EntityProp_AffectedByGravity = 1 << 7,
  EntityProp_CollidesWithGround = 1 << 8,
  EntityProp_Grounded = 1 << 9,
  EntityProp_FlashWhite = 1 << 10,
} EntityProp;

typedef enum MoveType
{
  MoveType_None,
  MoveType_Grounded,
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
  DrawType_Primitive,
  DrawType_Sprite,
} DrawType;

typedef struct EntityRef EntityRef;
struct EntityRef
{
  Entity *ptr;
  u64 id;
};

typedef struct Entity Entity;
struct Entity
{
  Entity *next;
  Entity *next_free;

  EntityRef parent;
  EntityRef *children;
  i16 child_count;
  i16 *free_child_list;
  i16 free_child_count;

  // General
  u64 id;
  u8 sp;
  EntityType type;
  MoveType move_type;
  CombatType combat_type;
  b64 props;
  bool is_active;
  bool marked_for_death;
  bool marked_for_spawn;

  // Transform
  Vec2F pos;
  f32 rot;
  Vec2F scale;
  Mat3x3F xform;
  Mat3x3F model_mat;
  Vec2F input_dir;

  // Physics
  Vec2F vel;
  Vec2F new_vel;
  f32 speed;
  
  // Drawing
  DrawType draw_type;
  Vec4F tint;
  TextureID texture;
  Vec2F dim;
  bool flip_x;
  bool flip_y;
  u16 z_index;

  // Collision
  Entity *colliders[_Collider_Count];
  ColliderID col_id;
  P_ColliderType col_type;
  f32 radius;

  // Targeting
  bool has_target;
  Vec2F target_pos;
  f32 target_angle;
  f32 view_dist;

  i16 health;
  i16 damage;

  // Timers
  Timer attack_timer;
  Timer damage_timer;
  Timer kill_timer;

  // ParticleGroup
  Arena particle_arena;
  Particle *particles;
  ParticleDesc particle_desc;
  Timer particle_timer;
};

typedef struct EntityList EntityList;
struct EntityList
{
  Entity *head;
  Entity *tail;
  Entity *first_free;
  u16 count;
};

typedef struct EntityParams EntityParams;
struct EntityParams
{
  Entity *entity;
  EntityType type;
  u64 id;
  b64 props;
  Vec2F pos;
  Vec4F color;
  ParticleDesc particle_desc;
};

Entity *NIL_ENTITY = &(Entity) {0};

// @SpawnEntity //////////////////////////////////////////////////////////////////////////

Entity *create_entity(Game *game, EntityType type);

#define spawn_entity(game, type, ...) \
  _spawn_entity(game, type, (EntityParams) {.pos=v2f(0, 0), .color=D_WHITE, __VA_ARGS__ })

Entity *_spawn_entity(Game *game, EntityType type, EntityParams params);

#define kill_entity(en) en->marked_for_death = TRUE

// @GeneralEntity ////////////////////////////////////////////////////////////////////////

bool entity_has_prop(Entity *en, EntityProp prop);
void entity_add_prop(Entity *en, EntityProp prop);
void entity_rem_prop(Entity *en, EntityProp prop);

Vec2F pos_from_entity(Entity *en);
Vec2F pos_tl_from_entity(Entity *en);
Vec2F pos_tr_from_entity(Entity *en);
Vec2F pos_bl_from_entity(Entity *en);
Vec2F pos_br_from_entity(Entity *en);

Vec2F dim_from_entity(Entity *en);
Vec2F scale_from_entity(Entity *en);
f32 rot_from_entity(Entity *en);

bool flip_x_from_entity(Entity *en);
bool flip_y_from_entity(Entity *en);

void entity_look_at(Entity *en, Vec2F target_pos);
void set_entity_target(Entity *en, EntityRef target);
bool is_entity_valid(Entity *en);

void damage_entity(Game *game, Entity *sender, Entity *reciever);

// @EntityRef ////////////////////////////////////////////////////////////////////////////

EntityRef ref_from_entity(Entity *en);
Entity *entity_from_ref(EntityRef ref);

// @EntityList ///////////////////////////////////////////////////////////////////////////

Entity *alloc_entity(Game *game);
void free_entity(Game *game, Entity *en);
Entity *get_entity_of_id(Game *game, u64 id);
Entity *get_entity_of_sp(Game *game, u8 sp);

void attach_entity_child(Entity *en, Entity *child);
void attach_entity_child_at(Entity *en, Entity *child, u16 index);
void detach_entity_child(Entity *en, Entity *child);
Entity *get_entity_child_at(Entity *en, u16 index);
Entity *get_entity_child_of_id(Entity *en, u64 id);
Entity *get_entity_child_of_sp(Entity *en, u8 sp);
Entity *get_entity_child_of_type(Entity *en, EntityType type);

void entity_add_collider(Game *game, Entity *en, ColliderID col_id);

// @Particle /////////////////////////////////////////////////////////////////////////////

void create_particles(Entity *en, ParticleDesc desc);

// @Other ////////////////////////////////////////////////////////////////////////////////

P_CollisionParams collision_params_from_entity(Entity *en, Vec2F vel);
bool timeout(Timer timer, f64 t);
