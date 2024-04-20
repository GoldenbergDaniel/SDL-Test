#include <stdio.h>

#include "base/base_inc.h"

#include "gfx/draw.h"
#include "phys/physics.h"
#include "event.h"
#include "game.h"
#include "global.h"
#include "entity.h"

extern Global *GLOBAL;

// @InitEntity ///////////////////////////////////////////////////////////////////////////

void init_entity(Entity *en, EntityType type)
{
  en->type = type;
  en->xform = m3x3f(1.0f);
  en->scale = v2f(1.0f, 1.0f);
  en->dim = v2f(10.0f, 10.0f);
  en->active = TRUE;
  en->visible = TRUE;
  en->color = v4f(1.0f, 1.0f, 1.0f, 1.0f);

  init_timers(en);

  switch (type)
  {
    case EntityType_General:
    {
      en->draw_type = DrawType_Sprite;
    }
    break;
    case EntityType_Player:
    {
      en->props = EntityProp_Rendered | EntityProp_Collides | EntityProp_Controlled | EntityProp_Moves | EntityProp_Combatant | EntityProp_WrapAtEdge;
      en->draw_type = DrawType_Sprite;
      en->move_type = MoveType_Walking;
      en->combat_type = CombatType_Ranged;
      en->scale = v2f(1.0f, 1.0f);
      en->speed = PLAYER_SPEED;
      en->texture = D_SPRITE_COWBOY;
      en->col.vertex_count = 4;

      Timer *timer = get_timer(en, TIMER_COMBAT);
      timer->max_duration = 0.5f;
      timer->curr_duration = 0.0f;
      timer->should_tick = FALSE;
    }
    break;
    case EntityType_ZombieWalker:
    {
      en->props = EntityProp_Rendered | EntityProp_Moves | EntityProp_Combatant | EntityProp_Collides;
      en->draw_type = DrawType_Sprite;
      en->move_type = MoveType_Walking;
      en->combat_type = CombatType_Melee;
      en->texture = D_SPRITE_ZOMBIE;
      en->speed = 100.0f;
      en->view_dist = 350.0f;
      en->col.vertex_count = 4;

      Timer *timer = get_timer(en, TIMER_COMBAT);
      timer->max_duration = 1.0f;
      timer->curr_duration = 0.0f;
      timer->should_loop = TRUE;
      timer->should_tick = FALSE;
    }
    break;
    case EntityType_ZombieBird:
    {
      en->props = EntityProp_Rendered | EntityProp_Moves | EntityProp_Combatant;
      en->draw_type = DrawType_Triangle;
      en->move_type = MoveType_Flying;
      en->combat_type = CombatType_Ranged;
      en->pos = v2f(random_u32(0, WIDTH), random_u32(0, HEIGHT));
      en->scale = v2f(1.5f, 1.5f);
      en->speed = 100.0f;
      en->color = D_RED;
      en->view_dist = 350.0f;

      Timer *timer = get_timer(en, TIMER_COMBAT);
      timer->should_loop = TRUE;
    }
    break;
    case EntityType_Equipped:
    {
      en->props = EntityProp_Rendered | EntityProp_Equipped;
      en->draw_type = DrawType_Sprite;
      en->texture = D_SPRITE_GUN;
    }
    break;
    case EntityType_Laser:
    {
      en->props = EntityProp_Rendered | EntityProp_Moves;
      en->draw_type = DrawType_Rectangle;
      en->move_type = MoveType_Projectile;
      en->combat_type = CombatType_Melee;
      en->scale = v2f(5.0f, 0.3f);
    }
    break;
    case EntityType_Wall:
    {
      en->props = EntityProp_Rendered | EntityProp_Collides;
      en->draw_type = DrawType_Rectangle;
      en->col.vertex_count = 4;
      
      update_entity_collider(en);
    }
    break;
    case EntityType_Debug:
    {
      en->props = EntityProp_Rendered;
      en->draw_type = DrawType_Rectangle;

      en->color = D_YELLOW;
    }
    break;
    default:
    {
      fprintf(stderr, "ERROR: Failed to initialize entity. Invalid type!");
      fprintf(stderr, "Entity ID: %llu\n", en->id);
      fprintf(stderr, "Entity Type: %i\n", en->type);
      assert(FALSE);
    }
  }
}

void reset_entity(Entity *en)
{
  Entity *next = en->next;
  Entity *next_free = en->next_free;
  EntityRef *children = en->children;
  i16 *free_child_list = en->free_child_list;

  zero(*en, Entity);
  
  en->free_child_list = free_child_list;
  en->children = children;
  reset_entity_children(en);
  en->next_free = next_free;
  en->next = next;
}

inline
void reset_entity_children(Entity *en)
{
  for (u16 i = 0; i < MAX_ENTITY_CHILDREN; i++)
  {
    en->children[i].ptr = NIL_ENTITY;
    en->children[i].id = 0;
    en->free_child_list[i] = -1;
  }
}

// @SpawnEntity //////////////////////////////////////////////////////////////////////////

Entity *_spawn_entity(Game *game, EntityType type, EntityParams params)
{
  Entity *en = alloc_entity(game);
  init_entity(en, type);
  en->props |= params.props;
  en->pos = params.pos;
  en->color = params.color;

  return en;
}

void _kill_entity(Game *game, EntityParams params)
{
  Entity *en = params.entity;
  if (en == NULL)
  {
    en = get_entity_of_id(game, params.id);
  }

  free_entity(game, en);
}

// @EntityRef ////////////////////////////////////////////////////////////////////////////

inline
EntityRef ref_from_entity(Entity *en)
{
  return (EntityRef)
  {
    .ptr = en,
    .id = en->id
  };
}

inline
Entity *entity_from_ref(EntityRef ref)
{
  Entity *result = NIL_ENTITY;

  if (is_entity_valid(ref.ptr) && ref.ptr->id == ref.id)
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

    reset_entity_children(new_en);

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

  new_en->id = random_u64(1, UINT64_MAX);

  return new_en;
}

void free_entity(Game *game, Entity *en)
{
  EntityList *list = &game->entities;
  reset_entity(en);
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

// @EntityTree ///////////////////////////////////////////////////////////////////////////

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

// @MiscEntity ///////////////////////////////////////////////////////////////////////////

Vec2F pos_from_entity(Entity *en)
{
  Mat3x3F result = en->model_mat;

  Entity *parent = entity_from_ref(en->parent);
  while (is_entity_valid(parent))
  {
    result = mul_3x3f(parent->model_mat, result);
    parent = entity_from_ref(parent->parent);
  }

  return v2f(result.e[0][2], result.e[1][2]);
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

Vec2F offset_from_entity(Entity *en)
{
  Vec2F dim = dim_from_entity(en);
  Vec2F result = v2f(dim.width/2.0f * -en->origin.x, dim.height/2.0f * -en->origin.y);
  
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
bool is_entity_valid(Entity *en)
{
  return (en != NULL && en->type != EntityType_Nil);
}

inline
void resolve_entity_collision(Entity *a, Entity *b)
{
  Vec2F size = dim_from_entity(a);

  if (a->pos.y + a->vel.y - size.height/2.0f <= b->pos.y)
  {
    a->pos.y = b->pos.y + size.height/2.0f;
    a->vel.y = 0.0f;
    a->new_vel.y = 0.0f;
    a->grounded = TRUE;
  }
  else
  {
    a->grounded = FALSE;
  }
}

void wrap_entity_at_edges(Entity *en)
{
  Vec2F size = dim_from_entity(en);
  
  if (en->pos.x + size.width <= 0.0f)
  {
    en->pos.x = WIDTH;
  }
  else if (en->pos.x >= WIDTH)
  {
    en->pos.x = -(size.width);
  }
}

// @UpdateEntity /////////////////////////////////////////////////////////////////////////

void update_entity_collider(Entity *en)
{
  P_Collider *col = &en->col;
  Vec2F size = dim_from_entity(en);

  switch (col->type)
  {
    case P_ColliderType_Polygon: // 4 vertices
    {
      // col->vertices[0].x = en->pos.x - size.width/2.0f;
      // col->vertices[0].y = en->pos.y + size.height/2.0f;
      // col->vertices[1].x = en->pos.x + size.width/2.0f;
      // col->vertices[1].y = en->pos.y + size.height/2.0f;
      // col->vertices[2].x = en->pos.x + size.width/2.0f;
      // col->vertices[2].y = en->pos.y - size.height/2.0f;
      // col->vertices[3].x = en->pos.x - size.width/2.0f;
      // col->vertices[3].y = en->pos.y - size.height/2.0f;

      col->edges[0][0] = 0;
      col->edges[0][1] = 1;
      col->edges[1][0] = 1;
      col->edges[1][1] = 2;
      col->edges[2][0] = 2;
      col->edges[2][1] = 3;
      col->edges[3][0] = 3;
      col->edges[3][1] = 4;

      col->normals[0] = normal_2f(col->vertices[col->edges[0][0]], 
                                  col->vertices[col->edges[0][1]]);
      col->normals[1] = normal_2f(col->vertices[col->edges[1][0]], 
                                  col->vertices[col->edges[1][1]]);
      col->normals[2] = normal_2f(col->vertices[col->edges[2][0]], 
                                  col->vertices[col->edges[2][1]]);
      col->normals[3] = normal_2f(col->vertices[col->edges[3][0]], 
                                  col->vertices[col->edges[3][1]]);
    }
    break;
    default: 
    {
      fprintf(stderr, "ERROR: Failed to update collider. Invalid type!");
      assert(0);
    }
  }
}

void update_entity_xform(Game *game, Entity *en)
{
  Mat3x3F xform = m3x3f(1.0f);
  Entity *parent = entity_from_ref(en->parent);

  // Scale
  xform = mul_3x3f(scale_3x3f(en->scale.x, en->scale.y), xform);

  // Offset position based on origin
  {
    Vec2F size = dim_from_entity(en);
    Vec2F offset = v2f(size.width/2.0f * -en->origin.x, size.height/2.0f * -en->origin.y);
    xform = mul_3x3f(translate_3x3f(offset.x, offset.y), xform);
  }

  // Flip
  if (en->flip_x) xform = mul_3x3f(scale_3x3f(-1.0f, 1.0f), xform);
  if (en->flip_y) xform = mul_3x3f(scale_3x3f(1.0f, -1.0f), xform);

  // Rotate
  xform = mul_3x3f(rotate_3x3f(en->rot * RADIANS), xform);

  // Translate
  {
    Vec2F scale = v2f(1.0f, 1.0f);
    for (Entity *p = parent; is_entity_valid(p); p = entity_from_ref(p->parent))
    {
      scale = mul_2f(scale, p->scale);
    }

    xform = mul_3x3f(translate_3x3f(en->pos.x/scale.x, en->pos.y/scale.y), xform);
  }

  en->model_mat = xform;

  // Move to world space
  {
    Mat3x3F model = m3x3f(1.0f);
    for (Entity *p = parent; is_entity_valid(p); p = entity_from_ref(p->parent))
    {
      model = mul_3x3f(p->model_mat, model);
    }
    
    xform = mul_3x3f(model, xform);
  }

  xform = mul_3x3f(game->camera, xform);
  xform = mul_3x3f(GLOBAL->renderer.projection, xform);

  en->xform = xform;
  
  en->p0 = transform_3f(v3f(-5.0f,  5.0f, 1.0f), en->xform);
  en->p1 = transform_3f(v3f( 5.0f,  5.0f, 1.0f), en->xform);
  en->p2 = transform_3f(v3f( 5.0f, -5.0f, 1.0f), en->xform);
  en->p3 = transform_3f(v3f(-5.0f, -5.0f, 1.0f), en->xform);
}

// @Timer ////////////////////////////////////////////////////////////////////////////////

void init_timers(Entity *en)
{
  en->timers[TIMER_COMBAT] = (Timer)
  {
    .max_duration = 1.0f,
    .curr_duration = 1.0f,
    .should_tick = TRUE,
    .ticking = FALSE,
    .timeout = FALSE,
    .should_loop = FALSE
  };

  en->timers[TIMER_HEALTH] = (Timer)
  {
    .max_duration = 1.0f,
    .curr_duration = 1.0f,
    .should_tick = TRUE,
    .ticking = FALSE,
    .timeout = FALSE,
    .should_loop = FALSE
  };

  en->timers[TIMER_KILL] = (Timer)
  {
    .max_duration = 5.0f,
    .curr_duration = 5.0f,
    .should_tick = TRUE,
    .ticking = FALSE,
    .timeout = FALSE,
    .should_loop = FALSE
  };
}

inline
Timer *get_timer(Entity *en, u8 index)
{
  return &en->timers[index];
}

bool tick_timer(Timer *timer, f64 dt)
{
  if (timer->ticking)
  {
    timer->curr_duration -= dt;

    if (timer->curr_duration <= 0.0f)
    {
      timer->timeout = TRUE;
      timer->ticking = FALSE;
      timer->should_tick = timer->should_loop;
    }
  }
  else
  {
    timer->curr_duration = timer->max_duration;
    timer->timeout = FALSE;
    timer->ticking = TRUE;
  }

  return timer->timeout;
}
