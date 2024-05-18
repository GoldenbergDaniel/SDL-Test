#include <stdio.h>

#include "base/base_inc.h"
#include "draw.h"
#include "game.h"
#include "global.h"
#include "entity.h"
#include "game.h"

extern Global *GLOBAL;
extern PrefabStore *PREFAB;

// @SpawnKillEntity //////////////////////////////////////////////////////////////////////

Entity *create_entity(Game *game, EntityType type)
{
  Entity *en = alloc_entity(game);
  en->type = type;
  en->is_active = TRUE;
  en->xform = m3x3f(1.0f);
  en->scale = SPRITE_SCALE;
  en->dim = v2f(16.0f, 16.0f);
  en->tint = v4f(1.0f, 1.0f, 1.0f, 1.0f);

  switch (type)
  {
    case EntityType_Debug:
    {
      en->draw_type = DrawType_Primitive;
      en->scale = v2f(0.1f, 0.1f);
      en->tint = D_YELLOW;
      en->scale = v2f(1.0f, 1.0f);
    }
    break;
    case EntityType_Player:
    {
      en->props = EntityProp_Renders | 
                  EntityProp_Collides | 
                  EntityProp_Controlled | 
                  EntityProp_Moves | 
                  EntityProp_Fights | 
                  EntityProp_WrapsAtEdges | 
                  EntityProp_AffectedByGravity;

      en->draw_type = DrawType_Sprite;
      en->move_type = MoveType_Grounded;
      en->combat_type = CombatType_Ranged;
      en->origin = v2f(0.5f, 0.5f);
      en->dim = v2f(8, 16);
      en->speed = PLAYER_SPEED;
      en->texture = D_SPRITE_COWBOY;
      en->attack_timer.duration = PLAYER_ATTACK_COOLDOWN;

      en->colliders[Collider_Body].dim = dim_from_entity(en);
    }
    break;
    case EntityType_ZombieWalker:
    {
      en->props = EntityProp_Renders | 
                  EntityProp_Moves | 
                  EntityProp_Fights | 
                  EntityProp_Collides |
                  EntityProp_AffectedByGravity;

      en->draw_type = DrawType_Sprite;
      en->move_type = MoveType_Grounded;
      en->combat_type = CombatType_Melee;
      en->texture = D_SPRITE_ZOMBIE;
      en->speed = 100.0f;
      en->view_dist = 350.0f;
      en->dim = v2f(8, 16);
      en->origin = v2f(0.5f, 0.5f);
      en->scale = SPRITE_SCALE;
      en->attack_timer.duration = ENEMY_ATTACK_COOLDOWN;
      en->health = 3;

      en->colliders[Collider_Body].dim = dim_from_entity(en);

      en->colliders[Collider_Hit].dim = pos_from_entity(en);
      en->colliders[Collider_Hit].dim = dim_from_entity(en);
      en->colliders[Collider_Hit].dim.x /= 2;
      en->colliders[Collider_Hit].dim.y /= 2;;
      en->colliders[Collider_Hit].offset = v2f(10, 10);
    }
    break;
    case EntityType_Equipped:
    {
      en->props = EntityProp_Renders | 
                  EntityProp_Equipped;

      en->draw_type = DrawType_Sprite;
      en->texture = D_SPRITE_GUN;
      en->origin = v2f(0.5f, 0.5f);
      en->scale = v2f(1.0f, 1.0f);
    }
    break;
    case EntityType_Bullet:
    {
      en->props = EntityProp_Renders | 
                  EntityProp_Moves | 
                  EntityProp_Collides;
      
      en->draw_type = DrawType_Sprite;
      en->texture = D_SPRITE_BULLET;
      en->move_type = MoveType_Projectile;
      en->combat_type = CombatType_Melee;
      en->kill_timer.duration = BULLET_KILL_TIME;
      en->damage = 1;

      en->colliders[Collider_Hit].type = P_ColliderType_Circle;
      en->colliders[Collider_Hit].radius = 8;
      en->colliders[Collider_Hit].dim = v2f(8, 8);
    }
    break;
    case EntityType_Wall:
    {
      en->props = EntityProp_Collides;
    }
    break;
    case EntityType_ParticleGroup:
    {
      en->props = EntityProp_Renders;
    }
    break;
    default: break;
  }

  return en;
}

Entity *_spawn_entity(Game *game, EntityType type, EntityParams params)
{
  Entity *en = create_entity(game, type);
  en->pos = params.pos;
  en->tint = params.color;
  en->is_active = FALSE;
  en->marked_for_spawn = TRUE;
  entity_add_prop(en, params.props);

  if (type == EntityType_ParticleGroup)
  {
    create_particles(en, params.particle_desc);
  }
  
  return en;
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
  while (is_entity_valid(parent))
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
  Vec2F offset = mul_2f(dim, en->origin);
  result.x -= offset.x;
  result.y += offset.y;

  return result;
}

Vec2F pos_tr_from_entity(Entity *en)
{
  Vec2F result = pos_from_entity(en);
  Vec2F dim = dim_from_entity(en);
  Vec2F offset = mul_2f(dim, en->origin);
  result.x += offset.x;
  result.y += offset.y;

  return result;
}

Vec2F pos_bl_from_entity(Entity *en)
{
  Vec2F result = pos_from_entity(en);
  Vec2F dim = dim_from_entity(en);
  Vec2F offset = mul_2f(dim, en->origin);
  result.x -= offset.x;
  result.y -= offset.y;

  return result;
}

Vec2F pos_br_from_entity(Entity *en)
{
  Vec2F result = pos_from_entity(en);
  Vec2F dim = dim_from_entity(en);
  Vec2F offset = mul_2f(dim, en->origin);
  result.x += offset.x;
  result.y -= offset.y;

  return result;
}

Vec2F dim_from_entity(Entity *en)
{
  Vec2F result = en->dim;
  result = mul_2f(result, en->scale);

  Entity *parent = entity_from_ref(en->parent);
  while (is_entity_valid(parent))
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
  while (is_entity_valid(parent))
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
  while (is_entity_valid(parent))
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
  while (is_entity_valid(parent))
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
  while (is_entity_valid(parent))
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
    en->target_pos = V2F_ZERO; // Note(dg): why go back to origin?
    en->has_target = FALSE;
  }
}

inline
bool is_entity_valid(Entity *en)
{
  return (en != NULL && en->type != EntityType_Nil);
}

void damage_entity(Game *game, Entity *reciever, Entity *sender)
{
  reciever->health -= sender->damage;
  if (reciever->health <= 0)
  {
    switch (reciever->type)
    {
      case EntityType_ZombieWalker:
      {
        spawn_entity(game, EntityType_ParticleGroup,
                      .pos=pos_from_entity(reciever),
                      .particle_desc=PREFAB->particle.death);
        
        kill_entity(reciever);
      }
      default: break;
    }
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

  if (is_entity_valid(ref.ptr) && ref.ptr->id == ref.id && !ref.ptr->marked_for_death)
  {
    result = ref.ptr;
  }

  return result;
}

// @EntityList ///////////////////////////////////////////////////////////////////////////

Entity *alloc_entity(Game *game)
{
  EntityList *list = &game->entities;
  Entity *new_en = list->first_free;

  if (new_en == NULL)
  {
    new_en = arena_alloc(&game->entity_arena, sizeof (Entity));
    zero(*new_en, Entity);

    u64 children_size = sizeof (EntityRef) * MAX_ENTITY_CHILDREN;
    new_en->children = arena_alloc(&game->entity_arena, children_size);

    u64 free_list_size = sizeof (i16) * MAX_ENTITY_CHILDREN;
    new_en->free_child_list = arena_alloc(&game->entity_arena, free_list_size);

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

  new_en->id = random_u64(2, UINT64_MAX);

  return new_en;
}

void free_entity(Game *game, Entity *en)
{
  EntityList *list = &game->entities;

  if (en->type == EntityType_ParticleGroup)
  {
    arena_destroy(&en->particle_arena);
  }

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

Entity *get_entity_of_id(Game *game, u64 id)
{
  Entity *result = NIL_ENTITY;

  for (Entity *en = game->entities.head; en != NULL; en = en->next)
  {
    if (en->id == id)
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
      if (!is_entity_valid(slot))
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
  if (!is_entity_valid(entity_from_ref(slot)))
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

// @Particles ////////////////////////////////////////////////////////////////////////////

void create_particles(Entity *en, ParticleDesc desc)
{
  en->particle_desc = desc;
  en->particle_arena = arena_create(MiB(1));
  en->particles = arena_alloc(&en->particle_arena, sizeof (Particle) * desc.count);

  for (i32 i = 0; i < en->particle_desc.count; i++)
  {
    Particle *particle = &en->particles[i];
    particle->pos = en->pos;
    particle->scale = desc.scale;
    particle->dir = (f32) random_i32(-en->particle_desc.spread, en->particle_desc.spread);
    particle->rot = (f32) random_i32(-45, 45);
    particle->color = desc.color_primary;
    particle->vel = desc.vel;
    particle->speed = desc.speed;
  }
}

// @Timer ////////////////////////////////////////////////////////////////////////////////

inline
bool is_timer_done(Timer timer, f64 t)
{
  return t >= timer.end_time;
}
