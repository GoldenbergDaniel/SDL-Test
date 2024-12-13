#include <stdio.h>

#include "base/base.h"
#include "vecmath/vecmath.h"
#include "physics/physics.h"
#include "draw.h"
#include "game.h"
#include "prefabs.h"
#include "entity.h"
#include "game.h"

extern Globals global;
extern Prefabs prefab;
extern Game game;

// @SpawnKillEntity //////////////////////////////////////////////////////////////////////

Entity *create_entity(EntityType type)
{
  Entity *en = alloc_entity();
  en->type = type;
  en->is_active = TRUE;
  en->xform = m3x3f(1.0f);
  en->dim = v2f(16, 16);
  en->scale = v2f(1, 1);
  en->tint = v4f(1.0f, 1.0f, 1.0f, 1.0f);

  switch (type)
  {
  case EntityType_Debug:
    en->draw_type = DrawType_Primitive;
    en->scale = v2f(1/16.0f, 1/16.0f);
    en->tint = DEBUG_YELLOW;
    break;
  case EntityType_Player:
    en->props = EntityProp_Renders | 
                EntityProp_Collides | 
                EntityProp_Controlled | 
                EntityProp_Moves | 
                EntityProp_AffectedByGravity |
                EntityProp_CollidesWithGround;

    en->draw_type = DrawType_Sprite;
    en->move_type = MoveType_Grounded;
    en->combat_type = CombatType_Ranged;
    en->speed = PLAYER_SPEED;
    en->texture = prefab.texture.player_idle;
    en->scale = v2f(SPRITE_SCALE, SPRITE_SCALE);
    en->health = PLAYER_HEALTH;
    en->invincibility_timer.duration = PLAYER_INVINCIBILITY_TIMER;
    en->dim = v2f(7, 15);

    en->anims[Animation_Idle] = prefab.animation.player_idle;
    en->anims[Animation_Walk] = prefab.animation.player_walk;
    en->anims[Animation_Jump] = prefab.animation.player_jump;

    entity_add_collider(en, Collider_Body);
    en->cols[Collider_Body]->col_type = P_ColliderType_Rect;
    en->cols[Collider_Body]->pos = v2f(0, 0);
    en->cols[Collider_Body]->scale = v2f(0.5, 1);
    break;
  case EntityType_Zombie:
    en->props = EntityProp_Renders | 
                EntityProp_Moves | 
                EntityProp_Collides |
                EntityProp_AffectedByGravity |
                EntityProp_CollidesWithGround;
    
    en->draw_type = DrawType_Sprite;
    en->move_type = MoveType_Grounded;
    en->combat_type = CombatType_Melee;
    en->scale = v2f(SPRITE_SCALE, SPRITE_SCALE);
    break;
  case EntityType_Equipped:
    en->props = EntityProp_Equipped;

    en->draw_type = DrawType_Sprite;
    en->dim = v2f(16, 16);
    break;
  case EntityType_Bullet:
    en->props = EntityProp_Renders | 
                EntityProp_Moves | 
                EntityProp_Collides;
    
    en->draw_type = DrawType_Sprite;
    en->texture = prefab.texture.bullet;
    en->move_type = MoveType_Projectile;
    en->kill_timer.duration = BULLET_KILL_TIME;
    en->scale = v2f(SPRITE_SCALE, SPRITE_SCALE);

    entity_add_collider(en, Collider_Hit);
    en->cols[Collider_Hit]->col_id = Collider_Hit;
    en->cols[Collider_Hit]->col_type = P_ColliderType_Circle;
    en->cols[Collider_Hit]->radius = 1;
    en->cols[Collider_Hit]->draw_type = DrawType_Nil;
    en->cols[Collider_Hit]->dim = V2F_ZERO;
    break;
  case EntityType_Decoration:
    en->props = EntityProp_Renders;
    en->draw_type = DrawType_Sprite;
    break;
  case EntityType_Collectable:
    en->props = EntityProp_Renders |
                EntityProp_BobsOverTime |
                EntityProp_Collides;

    en->draw_type = DrawType_Sprite;
    en->scale = v2f(SPRITE_SCALE, SPRITE_SCALE);

    entity_add_collider(en, Collider_Hit);
    en->cols[Collider_Hit]->col_id = Collider_Hit;
    en->cols[Collider_Hit]->col_type = P_ColliderType_Circle;
    en->cols[Collider_Hit]->radius = 10;
    en->cols[Collider_Hit]->draw_type = DrawType_Nil;
    en->cols[Collider_Hit]->dim = V2F_ZERO;
    break;
  case EntityType_Egg:
    en->props = EntityProp_Renders;
    en->draw_type = DrawType_Sprite;
    en->texture = prefab.texture.egg_0;
    en->scale = v2f(SPRITE_SCALE, SPRITE_SCALE);
    break;
  case EntityType_Collider:
    en->props = EntityProp_Collides |
                EntityProp_Renders;
    en->draw_type = DrawType_Primitive;
    break;
  default: break;
  }

  return en;
}

Entity *spawn_entity(EntityType type, Vec2F pos)
{
  Entity *en = create_entity(type);
  en->pos = pos;

  entity_rem_prop(en, EntityProp_Renders);
  en->is_active = FALSE;
  en->marked_for_spawn = TRUE;

  logger_debug(str("Spawned entity of kind %i\n"), type);

  return en;
}

Entity *spawn_zombie(ZombieKind kind, Vec2F pos)
{
  Entity *en = create_entity(EntityType_Zombie);
  en->pos = pos;
  en->zombie_kind = kind;

  ZombieDesc desc = prefab.zombie[kind];
  en->health = desc.health;
  en->damage = desc.damage;
  en->speed = desc.speed;
  en->view_dist = 350.0f;
  en->attack_timer.duration = desc.attack_cooldown;

  switch (kind)
  {
  default: break;
  case ZombieKind_Walker:
    en->texture = prefab.texture.walker_idle;
    en->anims[Animation_Idle] = prefab.animation.walker_idle;
    en->anims[Animation_Walk] = prefab.animation.walker_walk;

    entity_add_collider(en, Collider_Body);
    en->cols[Collider_Body]->col_type = P_ColliderType_Rect;
    en->cols[Collider_Body]->pos = v2f(0, 0);
    en->cols[Collider_Body]->scale = v2f(0.5, 1);

    entity_add_collider(en, Collider_Hit);
    en->cols[Collider_Hit]->col_type = P_ColliderType_Rect;
    en->cols[Collider_Hit]->pos = v2f(en->dim.width, 0);
    en->cols[Collider_Hit]->scale = v2f(0.25, 0.5);
    break;
  case ZombieKind_Chicken:
    en->dim = v2f(10, 8);

    en->texture = prefab.texture.chicken_idle;
    en->anims[Animation_Idle] = prefab.animation.chicken_idle;
    en->anims[Animation_Walk] = prefab.animation.chicken_idle;
    en->anims[Animation_LayEgg] = prefab.animation.chicken_lay;

    entity_add_collider(en, Collider_Body);
    en->cols[Collider_Body]->col_type = P_ColliderType_Rect;
    en->cols[Collider_Body]->pos = v2f(0, pos_bl_from_entity(en).y);
    en->cols[Collider_Body]->scale = v2f(0.5, 0.5);

    entity_add_collider(en, Collider_Hit);
    en->cols[Collider_Hit]->col_type = P_ColliderType_Rect;
    en->cols[Collider_Hit]->pos = v2f(20, pos_bl_from_entity(en).y + 10);
    en->cols[Collider_Hit]->scale = v2f(0.25, 0.25);

    entity_add_prop(en, EntityProp_LaysEggs);
    break;
  }

  entity_rem_prop(en, EntityProp_Renders);
  en->is_active = FALSE;
  en->marked_for_spawn = TRUE;

  return en;
}

Entity *spawn_collectable(CollectableKind kind, Vec2F pos)
{
  CollectableDesc desc = prefab.collectable[kind];

  Entity *en = create_entity(EntityType_Collectable);
  en->pos = pos;
  en->item_kind = kind;
  en->texture = desc.texture;
  en->bobbing_range = v2f(en->pos.y - 5, en->pos.y + 5);
  en->bobbing_state = -1;

  entity_rem_prop(en, EntityProp_Renders);
  en->is_active = FALSE;
  en->marked_for_spawn = TRUE;

  return en;
}

Entity *spawn_particles(ParticleKind kind, Vec2F pos)
{
  Entity *en = create_entity(EntityType_Any);
  en->pos = pos;

  ParticleDesc desc = prefab.particle[kind];
  en->particle_desc = desc;

  for (u32 i = 0; i < en->particle_desc.count; i++)
  {
    Particle *particle = get_next_free_particle();
    particle->is_active = TRUE;
    particle->pos = en->pos;
    particle->scale = desc.scale;
    particle->dir = (f32) random_i32(-en->particle_desc.spread, en->particle_desc.spread);
    particle->rot = (f32) random_i32(-45, 45);
    particle->color = desc.color_primary;
    particle->vel = desc.vel;
    particle->speed = desc.speed;
    particle->owner = ref_from_entity(en);
  }

  return en;
}

void kill_entity(Entity *en)
{
  en->marked_for_death = TRUE;
  push_event(EventType_EntityKilled, (EventDesc) {.en=en, .type=en->type});
}

// @GeneralEntity ////////////////////////////////////////////////////////////////////////

inline
bool entity_has_prop(Entity *en, EntityProp prop)
{
  return (en->props & prop) != 0;
}

inline
void entity_add_prop(Entity *en, EntityProp prop)
{
  en->props |= prop;
}

inline
void entity_rem_prop(Entity *en, EntityProp prop)
{
  en->props &= ~prop;
}

Vec2F pos_from_entity(Entity *en)
{
  Mat3x3F mat = en->model_mat;

  Entity *parent = entity_from_ref(en->parent);
  while (entity_is_valid(parent))
  {
    mat = mul_3x3f(parent->model_mat, mat);
    parent = entity_from_ref(parent->parent);
  }

  return v2f(mat.e[0][2], mat.e[1][2]);
}

Vec2F pos_tl_from_entity(Entity *en)
{
  Vec2F result = pos_from_entity(en);
  Vec2F dim = dim_from_entity(en);
  Vec2F offset = scale_2f(dim, 0.5f);
  result.x -= offset.x;
  result.y += offset.y;

  return result;
}

Vec2F pos_tr_from_entity(Entity *en)
{
  Vec2F result = pos_from_entity(en);
  Vec2F dim = dim_from_entity(en);
  Vec2F offset = scale_2f(dim, 0.5f);
  result.x += offset.x;
  result.y += offset.y;

  return result;
}

Vec2F pos_bl_from_entity(Entity *en)
{
  Vec2F result = pos_from_entity(en);
  Vec2F dim = dim_from_entity(en);
  Vec2F offset = scale_2f(dim, 0.5f);
  result.x -= offset.x;
  result.y -= offset.y;

  return result;
}

Vec2F pos_br_from_entity(Entity *en)
{
  Vec2F result = pos_from_entity(en);
  Vec2F dim = dim_from_entity(en);
  Vec2F offset = scale_2f(dim, 0.5f);
  result.x += offset.x;
  result.y -= offset.y;

  return result;
}

Vec2F dim_from_entity(Entity *en)
{
  Vec2F result = en->dim;
  result = mul_2f(result, en->scale);

  Entity *parent = entity_from_ref(en->parent);
  while (entity_is_valid(parent))
  {
    result = mul_2f(result, parent->scale);
    parent = entity_from_ref(parent->parent);
  }

  return result;
}

Vec2F scale_from_entity(Entity *en)
{
  Vec2F result = en->scale;

  Entity *parent = entity_from_ref(en->parent);
  while (entity_is_valid(parent))
  {
    result = mul_2f(result, parent->scale);
    parent = entity_from_ref(parent->parent);
  }

  return result;
}

f32 rot_from_entity(Entity *en)
{
  f32 result = en->rot;

  Entity *parent = entity_from_ref(en->parent);
  while (entity_is_valid(parent))
  {
    result += parent->rot;
    parent = entity_from_ref(parent->parent);
  }

  return result;
}

bool flip_x_from_entity(Entity *en)
{
  bool result = en->flip_x;

  Entity *parent = entity_from_ref(en->parent);
  while (entity_is_valid(parent))
  {
    result = parent->flip_x ? !result : result;
    parent = entity_from_ref(parent->parent);
  }

  return result;
}

bool flip_y_from_entity(Entity *en)
{
  bool result = en->flip_y;

  Entity *parent = entity_from_ref(en->parent);
  while (entity_is_valid(parent))
  {
    result = parent->flip_y ? !result : result;
    parent = entity_from_ref(parent->parent);
  }

  return result;
}

void entity_look_at(Entity *en, Vec2F target_pos)
{
  Vec2F entity_pos = pos_from_entity(en);
  en->flip_x = entity_pos.x > target_pos.x ? TRUE : FALSE;
}

void set_entity_target(Entity *en, EntityRef target)
{
  Entity *target_entity = entity_from_ref(target);
  Vec2F target_pos = pos_from_entity(target_entity);

  if (distance_2f(en->pos, target_pos) <= en->view_dist)
  {
    Vec2F diff = v2f(target_pos.x - en->pos.x, target_pos.y - en->pos.y);
    en->target_angle = atan_2f(diff);
    en->has_target = TRUE;
  }
  else
  {
    en->target_pos = V2F_ZERO;
    en->has_target = FALSE;
  }
}

inline
bool entity_is_valid(Entity *en)
{
  return (en != NULL && en->type != EntityType_Nil);
}

void damage_entity(Entity *reciever, i16 damage)
{
  if (reciever->health <= 0) return;

  reciever->health -= damage;
  entity_add_prop(reciever, EntityProp_FlashWhite);

  if (reciever->health <= 0)
  {
    spawn_particles(ParticleKind_Death, pos_from_entity(reciever));
    kill_entity(reciever);
  }
}

// @EntityRef ////////////////////////////////////////////////////////////////////////////

inline
EntityRef ref_from_entity(Entity *en)
{
  return (EntityRef) {
    .ptr = en,
    .id = en->id
  };
}

inline
Entity *entity_from_ref(EntityRef ref)
{
  Entity *result = NIL_ENTITY;

  if (entity_is_valid(ref.ptr) && ref.ptr->id == ref.id && !ref.ptr->marked_for_death)
  {
    result = ref.ptr;
  }

  return result;
}

// @EntityList ///////////////////////////////////////////////////////////////////////////

Entity *alloc_entity(void)
{
  EntityList *list = &game.entities;
  Entity *new_en = list->first_free;

  if (new_en == NULL)
  {
    new_en = arena_push(&game.entity_arena, Entity, 1);
    zero(*new_en, Entity);

    new_en->children = arena_push(&game.entity_arena, EntityRef, MAX_ENTITY_CHILDREN);
    new_en->free_child_list = arena_push(&game.entity_arena, i16, MAX_ENTITY_CHILDREN);

    // Reset entity children
    for (u16 i = 0; i < MAX_ENTITY_CHILDREN; i++)
    {
      new_en->children[i].ptr = NIL_ENTITY;
      new_en->children[i].id = 0;
      new_en->free_child_list[i] = -1;
    }

    if (list->head == NULL)
    {
      list->head = new_en;
      list->tail = new_en;
    }

    new_en->next = NULL;
    list->tail->next = new_en;
    list->tail = new_en;
    list->count++;
  }
  else
  {
    list->first_free = list->first_free->next_free;
  }

  new_en->id = (u64) random_i32(2, UINT32_MAX-1);

  return new_en;
}

void free_entity(Entity *en)
{
  EntityList *list = &game.entities;

  // Reset entity
  // NOTE(dg): This is a load of crap.
  Entity *next = en->next;
  Entity *next_free = en->next_free;
  EntityRef *children = en->children;
  i16 *free_child_list = en->free_child_list;
  zero(*en, Entity);
  en->free_child_list = free_child_list;
  en->children = children;
  for (u16 i = 0; i < MAX_ENTITY_CHILDREN; i++)
  {
    en->children[i].ptr = NIL_ENTITY;
    en->children[i].id = 0;
    en->free_child_list[i] = -1;
  }
  en->next_free = next_free;
  en->next = next;

  en->next_free = list->first_free;
  list->first_free = en;
}

Entity *get_entity_of_id(u64 id)
{
  Entity *result = NIL_ENTITY;

  for (Entity *en = game.entities.head; en != NULL; en = en->next)
  {
    if (en->id == id)
    {
      result = en;
      break;
    }
  }

  return result;
}

Entity *get_entity_of_sp(u8 sp)
{
  Entity *result = NIL_ENTITY;

  for (Entity *en = game.entities.head; en != NULL; en = en->next)
  {
    if (en->sp == sp)
    {
      result = en;
      break;
    }
  }

  return result;
}

void attach_entity_child(Entity *en, Entity *child)
{
  assert(en->child_count <= MAX_ENTITY_CHILDREN);

  en->child_count++;

  if (en->free_child_count == 0)
  {
    for (u16 i = 0; i < en->child_count; i++)
    {
      Entity *slot = entity_from_ref(en->children[i]);
      if (!entity_is_valid(slot))
      {
        en->children[i] = ref_from_entity(child);
        break;
      }
    }
  }
  else
  {
    for (u16 i = 0; i < en->free_child_count; i++)
    {
      i16 slot = en->free_child_list[i];
      if (slot != -1)
      {
        en->children[slot] = ref_from_entity(child);
        en->free_child_list[i] = -1;
        en->free_child_count--;
        break;
      }
    }
  }

  child->parent = ref_from_entity(en);
}

void attach_entity_child_at(Entity *en, Entity *child, u16 index)
{ 
  EntityRef slot = en->children[index];
  if (!entity_is_valid(entity_from_ref(slot)))
  {
    slot = ref_from_entity(child);
    child->parent = ref_from_entity(en);
  }
}

void detach_entity_child(Entity *en, Entity *child)
{
  for (u16 i = 0; i < en->child_count; i++)
  {
    EntityRef *slot = &en->children[i];
    if (slot->id == child->id)
    {
      zero(*slot, EntityRef);

      if (i < en->child_count)
      {
        en->free_child_list[en->free_child_count] = i;
        en->free_child_count++;
      }

      en->child_count--;
      zero(child->parent, EntityRef);
      break;
    }
  }
}

inline
Entity *get_entity_child_at(Entity *en, u16 index)
{
  return entity_from_ref(en->children[index]);
}

Entity *get_entity_child_of_id(Entity *en, u64 id)
{
  Entity *result = NIL_ENTITY;

  for (u16 i = 0; i < MAX_ENTITY_CHILDREN; i++)
  {
    Entity *curr = entity_from_ref(en->children[i]);
    if (curr->id == id)
    {
      result = curr;
      break;
    }
  }

  return result;
}

Entity *get_entity_child_of_sp(Entity *en, u8 sp)
{
  Entity *result = NIL_ENTITY;

  for (u16 i = 0; i < MAX_ENTITY_CHILDREN; i++)
  {
    Entity *curr = entity_from_ref(en->children[i]);
    if (curr->sp == sp)
    {
      result = curr;
      break;
    }
  }

  return result;
}

Entity *get_entity_child_of_type(Entity *en, EntityType type)
{
  Entity *result = NIL_ENTITY;

  for (u16 i = 0; i < MAX_ENTITY_CHILDREN; i++)
  {
    Entity *curr = entity_from_ref(en->children[i]);
    if (curr->type == type)
    {
      result = curr;
      break;
    }
  }

  return result;
}

void entity_add_collider(Entity *en, ColliderID col_id)
{
  en->cols[col_id] = create_entity(EntityType_Collider);
  en->cols[col_id]->col_id = col_id;
  attach_entity_child(en, en->cols[col_id]);
}

// @Timer ////////////////////////////////////////////////////////////////////////////////

inline
void timer_start(Timer *timer, f64 duration)
{
  timer->end_time = game.t + duration;
  timer->ticking = TRUE;
}

inline
bool timer_timeout(Timer *timer)
{
  return timer->ticking && game.t >= timer->end_time;
}

inline
f64 timer_remaining(Timer *timer)
{
  return timer->end_time - game.t;
}

// @Misc /////////////////////////////////////////////////////////////////////////////////

inline
bool has_prop(b64 props, u64 prop)
{
  return (props & prop) != 0;
}

P_CollisionParams collision_params_from_entity(Entity *en, Vec2F vel)
{
  if (en == NULL) assert(0);

  P_CollisionParams result = {
    .type = en->col_type,
    .pos = pos_bl_from_entity(en),
    .dim = dim_from_entity(en),
    .vel = vel,
    .radius = en->radius,
  };

  return result;
}

void equip_weapon(Entity *en, WeaponKind kind)
{
  if (!entity_is_valid(en)) return;

  Entity *weapon_en = get_entity_child_of_sp(en, SP_Gun);
  Entity *shot_point_en = get_entity_child_at(weapon_en, 0);

  if (kind == WeaponKind_Nil)
  {
    en->is_weapon_equipped = FALSE;
    Entity *gun = get_entity_child_of_sp(en, SP_Gun);
    entity_rem_prop(gun, EntityProp_Renders);
    return;
  }

  WeaponDesc desc = prefab.weapon[kind];

  if (weapon_en->weapon_kind != kind)
  {
    game.weapon.ammo_remaining = desc.ammo;
  }

  en->is_weapon_equipped = TRUE;
  en->attack_timer.duration = desc.shot_cooldown;

  weapon_en->texture = desc.texture;
  weapon_en->weapon_kind = kind;
  weapon_en->pos = desc.ancor;
  weapon_en->damage = desc.damage;
  weapon_en->speed = desc.bullet_speed;
  entity_add_prop(weapon_en, EntityProp_Renders);

  shot_point_en->pos = desc.shot_point;
}
