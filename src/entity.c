#include <stdio.h>
#include <math.h>

#include "base/base_common.h"
#include "base/base_math.h"
#include "base/base_random.h"

#include "phys/phys.h"
#include "input.h"
#include "event.h"
#include "entity.h"
#include "game.h"

#define DEBUG

extern Global *GLOBAL;

static void print_lists(Game *game);
static void init_timers(Entity *entity);
static Timer *get_timer(Entity *entity, u8 index);
static bool tick_timer(Timer *timer, f64 dt);

const b64 PLAYER_PROPS = EntityProp_Rendered | EntityProp_Controlled | EntityProp_Movable
  | EntityProp_Combatant | EntityProp_Collides;

const b64 ENEMY_PROPS = EntityProp_Rendered | EntityProp_Autonomous | EntityProp_Hostile | EntityProp_Movable | EntityProp_Combatant;

const b64 LASER_PROPS = EntityProp_Rendered | EntityProp_Movable;

// @InitEntity =================================================================================

void init_entity(Entity *entity, EntityType type)
{
  entity->type = type;
  entity->xform = m3x3f(1.0f);
  entity->is_active = TRUE;
  entity->is_visible = TRUE;

  init_timers(entity);

  switch (type)
  {
    case EntityType_Player:
    {
      entity->move_type = MoveType_Walking;
      entity->combat_type = CombatType_Ranged;
      entity->props = PLAYER_PROPS;
      entity->pos = v2f(W_WIDTH / 2.0f, W_HEIGHT / 2.0f);
      entity->color = COLOR_WHITE;
      entity->col.vertex_count = 4;

      set_entity_scale(entity, v2f(4.0f, 4.0f));

      Timer *timer = get_timer(entity, TIMER_COMBAT);
      timer->max_duration = 0.25f;
      timer->curr_duration = 0.0f;
      timer->should_tick = FALSE;
      
    }
    break;
    case EntityType_EnemyShip:
    {
      entity->move_type = MoveType_Flying;
      entity->combat_type = CombatType_Ranged;
      entity->props = ENEMY_PROPS;
      entity->pos = v2f(random_u32(0, W_WIDTH), random_u32(0, W_HEIGHT));
      entity->speed = 100.0f;
      entity->color = COLOR_RED;
      entity->view_dist = 350;

      set_entity_scale(entity, v2f(1.5f, 1.5f));

      Timer *timer = get_timer(entity, TIMER_COMBAT);
      timer->should_loop = TRUE;
    }
    break;
    case EntityType_Laser:
    {
      entity->move_type = MoveType_Projectile;
      entity->combat_type = CombatType_Melee;
      entity->props = LASER_PROPS;
      set_entity_scale(entity, v2f(3.0f, 0.15f));
    }
    break;
    case EntityType_Wall:
    {
      entity->props = EntityProp_Rendered | EntityProp_Collides;
      entity->pos = v2f(W_WIDTH/2.0f, W_HEIGHT/2.0f + 100.0f);
      entity->color = COLOR_RED;
      entity->col.vertex_count = 4;
      
      set_entity_scale(entity, v2f(2.0f, 2.0f));
      update_entity_collider(entity);
    }
    break;
    case EntityType_DebugLine:
    {
      entity->props = EntityProp_Rendered;
      entity->color = COLOR_YELLOW;
    }
    break;
    default:
    {
      printf("ERROR: Failed to initialize entity. Invalid type!");
      printf("Entity ID: %llu\n", entity->id);
      printf("Entity Type: %i\n", entity->type);
      printf("Vertex Count: %i\n", entity->col.vertex_count);
      ASSERT(FALSE);
    }
  }

  entity->width = 20.0f * entity->scale.x;
  entity->height = 20.0f * entity->scale.y;
}

inline
void clear_entity(Entity *entity)
{
  Entity *next = entity->next;
  Entity *next_free = entity->next_free;
  zero(*entity);
  entity->next = next;
  entity->next_free = next_free;
}

// @UpdateEntity ===============================================================================

void update_entity_collider(Entity *entity)
{
  Collider2D *col = &entity->col;

  switch (col->type)
  {
    case ColliderType_Polygon:
    {
      switch (col->vertex_count)
      {
        case 3:
        {
          col->vertices[0].x = entity->pos.x;
          col->vertices[0].y = entity->pos.y + entity->height/2.0f;
          col->vertices[1].x = entity->pos.x + entity->width/2.0f;
          col->vertices[1].y = entity->pos.y + entity->height/2.0f;
          col->vertices[2].x = entity->pos.x + entity->width/2.0f;
          col->vertices[2].y = entity->pos.y - entity->height/2.0f;

          col->edges[0][0] = 0;
          col->edges[0][1] = 1;
          col->edges[1][0] = 1;
          col->edges[1][1] = 2;
          col->edges[2][0] = 2;
          col->edges[2][1] = 0;

          col->normals[0] = normal_2f(col->vertices[col->edges[0][0]], 
                                      col->vertices[col->edges[0][1]]);
          col->normals[1] = normal_2f(col->vertices[col->edges[1][0]], 
                                      col->vertices[col->edges[1][1]]);
          col->normals[2] = normal_2f(col->vertices[col->edges[2][0]], 
                                      col->vertices[col->edges[2][1]]);
        }
        case 4:
        {
          col->vertices[0].x = entity->pos.x - entity->width/2.0f;
          col->vertices[0].y = entity->pos.y + entity->height/2.0f;
          col->vertices[1].x = entity->pos.x + entity->width/2.0f;
          col->vertices[1].y = entity->pos.y + entity->height/2.0f;
          col->vertices[2].x = entity->pos.x + entity->width/2.0f;
          col->vertices[2].y = entity->pos.y - entity->height/2.0f;
          col->vertices[3].x = entity->pos.x - entity->width/2.0f;
          col->vertices[3].y = entity->pos.y - entity->height/2.0f;

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
          printf("ERROR: Invalid number of vertices in collider!\n");
          printf("Entity ID: %llu\n", entity->id);
          printf("Entity Type: %i\n", entity->type);
          printf("Vertex Count: %i\n", entity->col.vertex_count);
          ASSERT(FALSE);
        }
      }
    }
    break;
    case ColliderType_Circle:
    {

    }
    break;
    default: 
    {
      printf("ERROR: Failed to update collider. Invalid type!");
      ASSERT(FALSE);
    }
  }
}

void update_entity_xform(Game *game, Entity *entity)
{
  Mat3x3F xform = m3x3f(1.0f);
  Entity *parent = entity_from_ref(entity->parent);

  if (entity_is_valid(parent))
  {
    xform = mul_3x3f(scale_3x3f(entity->scale.x, entity->scale.y), xform);
    xform = mul_3x3f(translate_3x3f(entity->pos_offset.x, entity->pos_offset.y), xform);
    xform = mul_3x3f(rotate_3x3f(entity->rot * RADIANS), xform);
    xform = mul_3x3f(translate_3x3f(entity->local_pos.x, entity->local_pos.y), xform);
    if (entity->flip_x) 
      xform = mul_3x3f(scale_3x3f(-1.0f, 1.0f), xform);
    
    xform = mul_3x3f(parent->model, xform);
  }
  else
  {
    xform = mul_3x3f(scale_3x3f(entity->scale.x, entity->scale.y), xform);
    xform = mul_3x3f(translate_3x3f(entity->pos_offset.x, entity->pos_offset.y), xform);
    xform = mul_3x3f(rotate_3x3f(entity->rot * RADIANS), xform);
    if (entity->flip_x) 
      xform = mul_3x3f(scale_3x3f(-1.0f, 1.0f), xform);

    xform = mul_3x3f(translate_3x3f(entity->pos.x, entity->pos.y), xform);
  }

  entity->model = xform;

  Mat3x3F camera = game->camera;
  xform = mul_3x3f(camera, xform);

  Mat3x3F ortho = orthographic_3x3f(0.0f, W_WIDTH, 0.0f, W_HEIGHT);
  xform = mul_3x3f(ortho, xform);

  entity->xform = xform;
}

void update_entity_walking_movement(Game *game, Entity *entity)
{
  f64 dt = game->dt;

  if (key_pressed(KEY_A) && !key_pressed(KEY_D))
  {
    entity->speed = lerp_1f(entity->speed, -PLAYER_SPEED, PLAYER_ACC * dt);
    entity->input_dir.x = -1.0f;
    // entity->rot = 180.0f;
    entity->flip_x = TRUE;
  }

  if (key_pressed(KEY_D) && !key_pressed(KEY_A))
  {
    entity->speed = lerp_1f(entity->speed, PLAYER_SPEED, PLAYER_ACC * dt);
    entity->input_dir.x = 1.0f;
    // entity->rot = 0.0f;
    entity->flip_x = FALSE;
  }

  if (key_pressed(KEY_A) && key_pressed(KEY_D))
  {
    entity->speed = lerp_1f(entity->speed, 0.0f, PLAYER_FRIC * 3.0f * dt);
    entity->speed = to_zero(entity->speed, 1.0f);
  }
  
  if (!key_pressed(KEY_A) && !key_pressed(KEY_D))
  {
    entity->speed = lerp_1f(entity->speed, 0.0f, PLAYER_FRIC * dt);
    entity->speed = to_zero(entity->speed, 1.0f);
  }

  entity->vel.x = entity->speed * dt;
  // entity->vel.y -= GRAVITY * dt;

  entity->pos = add_2f(entity->pos, entity->vel);
}

void update_entity_sliding_movement(Game *game, Entity *entity)
{
  f64 dt = game->dt;
  f32 omega = clamp(90.0f * magnitude_2f(entity->vel), 100.0f, 180.0f);
  // printf("Omega: %f\n", omega);

  if (key_pressed(KEY_A)) entity->rot += omega * dt;
  if (key_pressed(KEY_D)) entity->rot -= omega * dt;

  if (key_pressed(KEY_W) && !key_pressed(KEY_S))
  {
    entity->speed = lerp_1f(entity->speed, PLAYER_SPEED, PLAYER_ACC * dt);
  }
  else if (!key_pressed(KEY_W) && key_pressed(KEY_S))
  {
    entity->speed = lerp_1f(entity->speed, 0.0f, PLAYER_FRIC * 2 * dt);
    entity->speed = to_zero(entity->speed, 1.0f);
  }
  else
  {
    entity->speed = lerp_1f(entity->speed, 0.0f, PLAYER_FRIC * dt);
    entity->speed = to_zero(entity->speed, 1.0f);
  }

  entity->vel.x = cosf(entity->rot * RADIANS) * entity->speed * dt;
  entity->vel.y = sinf(entity->rot * RADIANS) * entity->speed * dt;

  entity->dir = scale_2f(entity->vel, 1.0f / magnitude_2f(entity->vel));
  entity->pos = add_2f(entity->pos, entity->vel);
}

void update_entity_flying_movement(Game *game, Entity *entity)
{
  f64 dt = game->dt;

  if (entity->has_target && entity->is_active)
  {
    entity->input_dir.x = sinf(entity->target_angle);
    entity->input_dir.y = cosf(entity->target_angle);

    entity->rot = -(entity->target_angle * DEGREES + 270.0f);
  }
  else
  {
    entity->input_dir = V2F_ZERO;
  }

  if (entity->input_dir.x != 0.0f || entity->input_dir.y != 0.0f)
  {
    entity->input_dir = normalize_2f(entity->input_dir);
  }

  // X Acceleration
  if (entity->input_dir.x != 0.0f)
  {
    entity->vel.x += PLAYER_ACC * dir(entity->input_dir.x) * dt;
    entity->vel.x = clamp(
                          entity->vel.x, 
                          -entity->speed * abs(entity->input_dir.x) * dt,
                          entity->speed * abs(entity->input_dir.x) * dt);
  }
  else
  {
    entity->vel.x = lerp_1f(entity->vel.x, 0.0f, PLAYER_FRIC * dt);
    entity->vel.x = to_zero(entity->vel.x, 0.1f);
  }

  // Y Acceleration
  if (entity->input_dir.y != 0.0f)
  {
    entity->vel.y += PLAYER_ACC * dir(entity->input_dir.y) * dt;
    entity->vel.y = clamp(
                          entity->vel.y, 
                          -entity->speed * abs(entity->input_dir.y) * dt, 
                          entity->speed * abs(entity->input_dir.y) * dt);
  }
  else 
  {
    entity->vel.y = lerp_1f(entity->vel.y, 0.0f, PLAYER_FRIC * dt);
    entity->vel.y = to_zero(entity->vel.y, 0.1f);
  }

  entity->dir = scale_2f(entity->vel, 1.0f / magnitude_2f(entity->vel));
  entity->pos = add_2f(entity->pos, entity->vel);

  // printf("Dir: %.2f, %.2f\n", entity->dir.x, entity->dir.y);
}

void update_entity_projectile_movement(Game *game, Entity *entity)
{
  f64 dt = game->dt;
  Timer *timer = get_timer(entity, TIMER_KILL);
  tick_timer(timer, dt);

  if (timer->timeout)
  {
    EventDesc desc = {.id = entity->id};
    push_event(game, EventType_KillEntity, desc);
  }

  entity->vel.x = cosf(entity->rot * RADIANS) * entity->speed * dt;
  entity->vel.y = sinf(entity->rot * RADIANS) * entity->speed * dt;

  entity->pos = add_2f(entity->pos, entity->vel);
}

void update_controlled_entity_combat(Game *game, Entity *entity)
{
  Timer *timer = get_timer(entity, TIMER_COMBAT);

  if (timer->should_tick)
  {
    tick_timer(timer, game->dt);
  }

  bool can_shoot = key_pressed(KEY_SPACE) && (timer->timeout || !timer->should_tick);
  if (can_shoot)
  {
    EventDesc desc = 
    {
      .type = EntityType_Laser,
      .position = v2f(entity->pos.x, entity->pos.y),
      .rotation = entity->rot,
      .speed = 1000.0f,
      .color = COLOR_BLUE
    };
    
    push_event(game, EventType_SpawnEntity, desc);

    timer->should_tick = TRUE;
  }
}

void update_targetting_entity_combat(Game *game, Entity *entity)
{
  if (entity->has_target)
  {
    Timer *timer = get_timer(entity, TIMER_COMBAT);
    tick_timer(timer, game->dt);

    bool can_shoot = timer->timeout;
    if (can_shoot)
    {
      EventDesc desc = 
      {
        .type = EntityType_Laser,
        .position = v2f(entity->pos.x, entity->pos.y),
        .rotation = entity->rot,
        .speed = 1000.0f,
        .color = COLOR_GREEN
      };

      push_event(game, EventType_SpawnEntity, desc);
    }
  }
}

// @SetEntity ==================================================================================

inline
void set_entity_size(Entity *entity, f32 width, f32 height)
{
  entity->width = width;
  entity->height = height;
  entity->scale = v2f(entity->width/20.0f, entity->height/20.0f);
}

inline
void set_entity_scale(Entity *entity, Vec2F scale)
{
  entity->scale = scale;
  entity->width = 20.0f * entity->scale.x;
  entity->height = 20.0f * entity->scale.y;
}

void set_entity_origin(Entity *entity, Vec2I origin)
{
  if (origin.x == 0 && origin.y == 0)
  {
    entity->pos_offset = V2F_ZERO;
  }
  else if (origin.x == -1 && origin.y == 0)
  {
    entity->pos_offset = v2f(entity->width/2.0f, 0.0f);
  }
  else if (origin.x == 1 && origin.y == 0)
  {
    entity->pos_offset = v2f(-entity->width/2.0f, 0.0f);
  }
  else if (origin.x == -1 && origin.y == 1)
  {
    entity->pos_offset = v2f(entity->width/2.0f, -entity->height/2.0f);
  }
}

void set_entity_target(Entity *entity, EntityRef target)
{
  Entity *target_entity = entity_from_ref(target);
  
  if (target_entity == NULL) return;

  Vec2F target_pos = target_entity->pos;

  if (distance_2f(entity->pos, target_pos) <= entity->view_dist)
  {
    f32 dist_x = target_pos.x - entity->pos.x;
    f32 dist_y = target_pos.y - entity->pos.y;
    entity->target_angle = atan2(dist_x, dist_y);
    entity->has_target = TRUE;
  }
  else
  {
    entity->target_pos = V2F_ZERO;
    entity->has_target = FALSE;
  }
}

void flip_entity_x(Entity *entity)
{
  
}

void flip_entity_y(Entity *entity)
{

}

// @OtherEntity ================================================================================

inline
bool entity_is_valid(Entity *entity)
{
  return (entity != NULL && entity->type != EntityType_Nil);
}

inline
void resolve_entity_collision(Entity *a, Entity *b)
{

}

void wrap_entity_at_edges(Entity *entity)
{
  if (entity->pos.x + entity->width <= 0.0f)
  {
    entity->pos.x = W_WIDTH;
  }
  else if (entity->pos.x >= W_WIDTH)
  {
    entity->pos.x = -(entity->width);
  }
  else if (entity->pos.y + entity->height <= 0.0f)
  {
    entity->pos.y = W_HEIGHT;
  }
  else if (entity->pos.y >= W_HEIGHT)
  {
    entity->pos.y = -(entity->height);
  }
}

void damage_entity(Entity *entity, i8 damage)
{
  entity->curr_health -= damage;

  if (entity->curr_health <= 0)
  {
    entity->curr_health = 0;
    entity->is_active = FALSE;
    entity->is_visible = FALSE;
    printf("Player ded\n");
  }
}

// @EntityRef ==================================================================================

inline
EntityRef ref_from_entity(Entity *entity)
{
  return (EntityRef) {entity, entity->id};
}

inline
Entity *entity_from_ref(EntityRef ref)
{
  Entity *result = GLOBAL->nil_entity;

  if (ref.ptr != NULL && ref.ptr->id == ref.id)
  {
    result = ref.ptr;
  }

  return result;
}

// @EntityList =================================================================================

Entity *create_entity(Game *game)
{
  EntityList *list = &game->entities;
  Entity *new_entity = list->first_free;

  if (new_entity == NULL)
  {
    new_entity = arena_alloc(&game->arena, sizeof (Entity));
    clear_entity(new_entity);

    if (list->head == NULL)
    {
      list->head = new_entity;
    }

    if (list->tail != NULL)
    {
      list->tail->next = new_entity;
    }

    new_entity->next = NULL;
    list->tail = new_entity;
    list->count++;
  }
  else
  {
    list->first_free = list->first_free->next_free;
  }

  new_entity->id = random_u64(1, UINT64_MAX);

  return new_entity;
}

void destroy_entity(Game *game, Entity *entity)
{
  EntityList *list = &game->entities;
  clear_entity(entity);
  entity->next_free = list->first_free;
  list->first_free = entity;
}

Entity *get_entity_of_id(Game *game, u64 id)
{
  Entity *result = GLOBAL->nil_entity;
  
  for (Entity *e = game->entities.head; e != NULL; e = e->next)
  {
    if (e->id == id)
    {
      result = e;
      break;
    }
  }

  return result;
}

Entity *get_nearest_entity_of_type(Game *game, Vec2F pos, EntityType type)
{
  Entity *result = GLOBAL->nil_entity;
  f32 min_dist = FLT_MAX;

  for (Entity *e = game->entities.head; e != NULL; e = e->next)
  {
    if (e->type == type)
    {
      f32 dist = distance_2f(e->pos, pos);

      if (dist < min_dist)
      {
        result = e;
        min_dist = dist;
      }
    }
  }

  return result;
}

// @EntityTree =================================================================================

void set_entity_parent(Entity *entity, Entity *parent)
{
  entity->parent = ref_from_entity(parent);

  if (parent->first_child == NULL)
  {
    parent->first_child = entity;
  }
  else
  {
    parent->last_child->next_child = entity;
  }

  parent->last_child = entity;
}

void add_entity_child(Entity *entity, Entity *child)
{
  entity->last_child->next_child = child;
  entity->last_child = child;
  child->parent = ref_from_entity(entity);
}

// NOTE: NEED TO TEST!
void remove_entity_child(Entity *entity, u64 id)
{
  Entity *prev = NULL;

  for (Entity *curr = entity->first_child; curr != NULL; curr = curr->next_child)
  {
    if (curr->id == id)
    {
      zero(curr->parent);
      prev->next_child = curr->next_child;
      break;
    }

    prev = curr;
  }
}

Entity *get_entity_child_at_index(Entity *entity, u8 index)
{
  Entity *result = GLOBAL->nil_entity;

  u8 i = 0;
  for (Entity *curr = entity->first_child; curr != NULL; curr = curr->next_child)
  {
    if (i == index)
    {
      result = curr;
      break;
    }

    i++;
  }

  return result;
}

Entity *get_entity_child_of_id(Entity *entity, u64 id)
{
  Entity *result = {0};

  for (Entity *curr = entity->first_child; curr != NULL; curr = curr->next_child)
  {
    if (curr->id == id)
    {
      result = curr;
      break;
    }
  }

  return result;
}

Entity *get_entity_child_of_type(Entity *entity, EntityType type)
{
  Entity *result = {0};

  for (Entity *curr = entity->first_child; curr != NULL; curr = curr->next_child)
  {
    if (curr->type == type)
    {
      result = curr;
      break;
    }
  }

  return result;
}

// @Timer ======================================================================================

static
void init_timers(Entity *entity)
{
  entity->timers[TIMER_COMBAT] = (Timer)
  {
    .max_duration = 1.0f,
    .curr_duration = 1.0f,
    .should_tick = TRUE,
    .is_ticking = FALSE,
    .timeout = FALSE,
    .should_loop = FALSE
  };

  entity->timers[TIMER_HEALTH] = (Timer)
  {
    .max_duration = 1.0f,
    .curr_duration = 1.0f,
    .should_tick = TRUE,
    .is_ticking = FALSE,
    .timeout = FALSE,
    .should_loop = FALSE
  };

  entity->timers[TIMER_KILL] = (Timer)
  {
    .max_duration = 5.0f,
    .curr_duration = 5.0f,
    .should_tick = TRUE,
    .is_ticking = FALSE,
    .timeout = FALSE,
    .should_loop = FALSE
  };
}

static inline
Timer *get_timer(Entity *entity, u8 index)
{
  return &entity->timers[index];
}

static
bool tick_timer(Timer *timer, f64 dt)
{
  if (timer->is_ticking)
  {
    timer->curr_duration -= dt;

    if (timer->curr_duration <= 0.0f)
    {
      timer->timeout = TRUE;
      timer->is_ticking = FALSE;
      timer->should_tick = timer->should_loop;
    }
  }
  else
  {
    timer->curr_duration = timer->max_duration;
    timer->timeout = FALSE;
    timer->is_ticking = TRUE;
  }

  return timer->timeout;
}
