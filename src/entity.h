#pragma once

#include "base/base.h"
#include "physics/physics.h"
#include "draw.h"

#define MAX_ENTITY_CHILDREN 16

#define WALKER_SPEED 50.0f

#define PLAYER_SPEED 400.0f
#define PLAYER_JUMP_VEL 900.0f
#define PLAYER_ACC 3.0f
#define PLAYER_FRIC 8.0f
#define PLAYER_HEALTH 3

#define BULLET_KILL_TIME 5.0f
#define PLAYER_ATTACK_COOLDOWN 0.5f
#define ENEMY_ATTACK_COOLDOWN 1.0f
#define FLASH_TIME 0.05f

enum
{
  SP_Nil,
  SP_Player,
  SP_Gun,
};

typedef enum ColliderID
{
  Collider_Body,
  Collider_Head,
  Collider_Hit,

  Collider_COUNT,
} ColliderID;

typedef struct EntityRef EntityRef;
struct EntityRef
{
  Entity *ptr;
  u64 id;
};

// @Animation ////////////////////////////////////////////////////////////////////////////

typedef enum AnimationState
{
  Animation_None,
  Animation_Idle,
  Animation_Walk,
  Animation_Jump,

  Animation_COUNT,
} AnimationState;

typedef struct Animation Animation;
struct Animation
{
  u8 frame_idx;
  u16 tick_counter;
};

typedef struct AnimationDesc AnimationDesc;
struct AnimationDesc
{
  TextureID frames[5];
  u8 frame_count;
  u16 ticks_per_frame;
};

// @Timer ////////////////////////////////////////////////////////////////////////////////

typedef struct Timer Timer;
struct Timer
{
  f32 duration;
  f64 end_time;
  bool is_ticking;
};

// @Particle /////////////////////////////////////////////////////////////////////////////

typedef enum ParticleKind
{
  ParticleKind_Smoke,
  ParticleKind_Blood,
  ParticleKind_Death,
  ParticleKind_PickupCoin,
  ParticleKind_Debug,

  ParticleKind_COUNT,
} ParticleKind;

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

typedef struct Particle Particle;
struct Particle
{
  EntityRef owner;

  Vec4F color;
  Vec2F pos;
  Vec2F scale;
  f32 dir;
  f32 rot;
  Vec2F vel;
  f32 speed;
  bool is_grounded;
  bool is_active;
};

// @Entity ///////////////////////////////////////////////////////////////////////////////

typedef enum EntityType
{
  EntityType_Nil,
  EntityType_Debug,
  EntityType_Player,
  EntityType_Zombie,
  EntityType_Equipped,
  EntityType_Bullet,
  EntityType_Collider,
  EntityType_ParticleGroup,
  EntityType_Collectable,
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
  EntityProp_BobsOverTime = 1 << 9,
  EntityProp_Grounded = 1 << 10,
  EntityProp_FlashWhite = 1 << 11,
  EntityProp_Zombie = 1 << 12,
} EntityProp;

typedef enum WeaponKind
{
  WeaponKind_Nil,
  WeaponKind_Pistol,
  WeaponKind_Rifle,
  WeaponKind_Shotgun,
  WeaponKind_SMG,

  WeaponKind_COUNT,
} WeaponKind;

typedef struct WeaponDesc WeaponDesc;
struct WeaponDesc
{
  TextureID texture;
  Vec2F ancor;
  Vec2F shot_point;
  f32 shot_cooldown;
  f32 bullet_speed;
  u16 damage;
};

typedef enum CollectableKind
{
  CollectableKind_Nil,
  CollectableKind_Coin,
  CollectableKind_Soul,

  CollectableKind_COUNT,
} CollectableKind;

typedef struct CollectableDesc CollectableDesc;
struct CollectableDesc
{
  TextureID texture;
  i32 draw_chance;
};

typedef enum ZombieKind
{
  ZombieKind_Nil,
  ZombieKind_Walker,
  ZombieKind_Chicken,

  ZombieKind_COUNT,
} ZombieKind;

typedef struct ZombieDesc ZombieDesc;
struct ZombieDesc
{
  EntityProp props;
  u16 speed;
  u16 health;
  u16 damage;
  u16 cost;
};

typedef enum MoveType
{
  MoveType_Nil,
  MoveType_Grounded,
  MoveType_Projectile,
  MoveType_Flying,
} MoveType;

typedef enum CombatType
{
  CombatType_Nil,
  CombatType_Melee,
  CombatType_Ranged,
} CombatType;

typedef enum DrawType
{
  DrawType_Nil,
  DrawType_Primitive,
  DrawType_Sprite,
} DrawType;

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
  Entity *cols[Collider_COUNT];
  ColliderID col_id;
  P_ColliderType col_type;
  f32 radius;
  bool colliding_with_player;

  // Animation
  Animation anim;
  AnimationDesc anims[Animation_COUNT];
  AnimationState anim_state;
  AnimationState anim_state_prev;
  Vec2F bobbing_range;
  i8 bobbing_state;

  Timer flash_timer;

  // Targeting
  bool has_target;
  Vec2F target_pos;
  f32 target_angle;
  f32 view_dist;

  // ParticleGroup
  ParticleDesc particle_desc;
  Timer particle_timer;

  // Combat
  i16 health;
  i16 damage;
  Timer attack_timer;
  Timer damage_timer;
  Timer kill_timer;
  Timer invincibility_timer;
  bool weapon_equipped;

  ZombieKind zombie_kind;
  WeaponKind weapon_kind;
  CollectableKind item_kind;
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
  b64 props;
  Vec2F pos;
  Vec4F tint;
  ParticleDesc particle_desc;
};

Entity *NIL_ENTITY = &(Entity) {0};

// @SpawnEntity //////////////////////////////////////////////////////////////////////////

Entity *create_entity(EntityType type);

#define spawn_entity(type, ...) \
  _spawn_entity(type, (EntityParams) {.pos=v2f(0, 0), .tint=DEBUG_WHITE, .props=0, __VA_ARGS__ })

Entity *_spawn_entity(EntityType type, EntityParams params);
Entity *spawn_zombie(ZombieKind kind);
Entity *spawn_particles(ParticleKind kind, Vec2F pos);

void kill_entity(Entity *en);

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
bool entity_is_valid(Entity *en);

void damage_entity(Entity *reciever, i16 damage);

// @EntityRef ////////////////////////////////////////////////////////////////////////////

EntityRef ref_from_entity(Entity *en);
Entity *entity_from_ref(EntityRef ref);

// @EntityList ///////////////////////////////////////////////////////////////////////////

Entity *alloc_entity(void);
void free_entity(Entity *en);
Entity *get_entity_of_id(u64 id);
Entity *get_entity_of_sp(u8 sp);

void attach_entity_child(Entity *en, Entity *child);
void attach_entity_child_at(Entity *en, Entity *child, u16 index);
void detach_entity_child(Entity *en, Entity *child);
Entity *get_entity_child_at(Entity *en, u16 index);
Entity *get_entity_child_of_id(Entity *en, u64 id);
Entity *get_entity_child_of_sp(Entity *en, u8 sp);
Entity *get_entity_child_of_type(Entity *en, EntityType type);

void entity_add_collider(Entity *en, ColliderID col_id);

// @Particle /////////////////////////////////////////////////////////////////////////////

void create_particles(Entity *en, ParticleDesc desc);

// @Other ////////////////////////////////////////////////////////////////////////////////

P_CollisionParams collision_params_from_entity(Entity *en, Vec2F vel);
void timer_start(Timer *timer, f64 duration);
bool timer_timeout(Timer *timert);
void timer_reset(Timer *timer);

void equip_weapon(Entity *en, WeaponKind kind);
