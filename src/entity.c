#include <stdio.h>
#include <math.h>

#include "base/base_common.h"
#include "base/base_math.h"
#include "base/base_random.h"

#include "input.h"
#include "event.h"
#include "entity.h"
#include "game.h"

#define DEBUG

static void print_lists(Game *game);
static void init_timers(Entity *entity);
static Timer *get_timer(Entity *entity, u8 index);
static bool tick_timer(Timer *timer, f64 dt);

// @Entity =====================================================================================

void init_entity(Entity *entity, EntityType type)
{
  entity->type = type;
  entity->width = 20.0f * entity->scale.x;
  entity->height = 20.0f * entity->scale.y;
  entity->is_active = TRUE;
  entity->is_visible = TRUE;

  switch (type)
  {
    case EntityType_Player:
    {
      entity->props = PLAYER_PROPS;
      entity->pos = v2f(W_WIDTH / 2.0f, W_HEIGHT / 2.0f);
      entity->scale = v2f(1.0f, 1.0f);
      entity->speed = PLAYER_SPEED;
      entity->color = COLOR_WHITE;
    }
    break;
    case EntityType_EnemyShip:
    {
      entity->props = ENEMY_PROPS;
      entity->pos = v2f(random_u32(0, W_WIDTH), random_u32(0, W_HEIGHT));
      entity->scale = v2f(1.5f, 1.5f);
      entity->speed = 100.0f;
      entity->color = COLOR_RED;
      entity->view_dist = 350;
    }
    break;
    case EntityType_Laser:
    {
      entity->props = LASER_PROPS;
      entity->scale = v2f(3.0f, 0.15f);
    }
    break;
    default:
    {
      printf("ERROR: Failed to initialize entity. Invalid type!");
      ASSERT(FALSE);
    }
  }

  init_timers(entity);
}

inline
void clear_entity(Entity *entity)
{
  Entity *next = entity->next;
  Entity *next_free = entity->next_free;
  *entity = (Entity) {0};
  entity->next = next;
  entity->next_free = next_free;
}

void update_entity_xform(Game *game, Entity *entity)
{
  Mat3x3F xform = m3x3f(1.0f);
  xform = mul_3x3f(scale_3x3f(entity->scale.x, entity->scale.y), xform);
  xform = mul_3x3f(rotate_3x3f(entity->rot * RADIANS), xform);
  xform = mul_3x3f(translate_3x3f(entity->pos.x, entity->pos.y), xform);
  // xform = mul_3x3f(translate_3x3f(entity->width/2.0f, entity->height/2.0f), xform);
  entity->xform = xform;
}

void update_projectile_entity_movement(Game *game, Entity *entity)
{
  Timer *timer = get_timer(entity, TIMER_KILL);
  tick_timer(timer, game->dt);

  if (timer->timeout)
  {
    EventDesc desc = {.id = entity->id};
    push_event(game, EventType_KillEntity, desc);
  }

  entity->vel.x = cosf(entity->rot * RADIANS) * entity->speed * game->dt;
  entity->vel.y = sinf(entity->rot * RADIANS) * entity->speed * game->dt;

  entity->pos = add_2f(entity->pos, entity->vel);
}

void update_controlled_entity_movement(Game *game, Entity *entity)
{
  f64 dt = game->dt;

  if (key_pressed(KEY_A)) entity->rot += 200.0f * game->dt;
  
  if (key_pressed(KEY_D)) entity->rot -= 200.0f * game->dt;

  if (key_pressed(KEY_W) && !key_pressed(KEY_S))
  {
    entity->speed = lerp_1f(entity->speed, PLAYER_SPEED * 3.0f, PLAYER_ACC * dt);
  }
  else if (!key_pressed(KEY_W) && key_pressed(KEY_S))
  {
    entity->speed = lerp_1f(entity->speed, PLAYER_SPEED * 0.5f, PLAYER_FRIC * dt);
  }
  else
  {
    entity->speed = lerp_1f(entity->speed, PLAYER_SPEED, PLAYER_FRIC * dt);
  }

  entity->vel.x = cosf(entity->rot * RADIANS) * entity->speed * dt;
  entity->vel.y = sinf(entity->rot * RADIANS) * entity->speed * dt;

  entity->pos = add_2f(entity->pos, entity->vel);
}

void update_targetting_entity_movement(Game *game, Entity *entity)
{
  f64 dt = game->dt;

  if (entity->has_target && entity->is_active)
  {
    entity->dir.x = sinf(entity->target_angle);
    entity->dir.y = cosf(entity->target_angle);

    entity->rot = -(entity->target_angle * DEGREES + 270.0f);
  }
  else
  {
    entity->dir = V2F_ZERO;
  }

  if (entity->dir.x != 0.0f || entity->dir.y != 0.0f)
  {
    entity->dir = normalize_2f(entity->dir);
  }

  // X Acceleration
  if (entity->dir.x != 0.0f)
  {
    entity->vel.x += PLAYER_ACC * dir(entity->dir.x) * dt;
    entity->vel.x = clamp(
                          entity->vel.x, 
                          -entity->speed * abs(entity->dir.x) * dt,
                          entity->speed * abs(entity->dir.x) * dt);
  }
  else
  {
    entity->vel.x = lerp_1f(entity->vel.x, 0.0f, PLAYER_FRIC * dt);
    entity->vel.x = to_zero(entity->vel.x, 0.1f);
  }

  // Y Acceleration
  if (entity->dir.y != 0.0f)
  {
    entity->vel.y += PLAYER_ACC * dir(entity->dir.y) * dt;
    entity->vel.y = clamp(
                          entity->vel.y, 
                          -entity->speed * abs(entity->dir.y) * dt, 
                          entity->speed * abs(entity->dir.y) * dt);
  }
  else 
  {
    entity->vel.y = lerp_1f(entity->vel.y, 0.0f, PLAYER_FRIC * dt);
    entity->vel.y = to_zero(entity->vel.y, 0.1f);
  }

  entity->pos = add_2f(entity->pos, entity->vel);
}

void update_controlled_entity_combat(Game *game, Entity *entity)
{
  Timer *timer = get_timer(entity, TIMER_COMBAT);
  timer->should_loop = FALSE;
  timer->start_duration = 0.25f;
  tick_timer(timer, game->dt);

  if (key_pressed(KEY_SPACE) && timer->timeout)
  {
    Vec2F shot_point = v2f(entity->pos.x, entity->pos.y);
    EventDesc desc = 
    {
      .type = EntityType_Laser,
      .position = shot_point,
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
    timer->should_loop = TRUE;
    tick_timer(timer, game->dt);

    if (timer->timeout)
    {
      Vec2F shot_point = v2f(entity->pos.x, entity->pos.y);
      EventDesc desc = 
      {
        .type = EntityType_Laser,
        .position = shot_point,
        .rotation = entity->rot,
        .speed = 1000.0f,
        .color = COLOR_GREEN
      };

      push_event(game, EventType_SpawnEntity, desc);
    }
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
  entity->health -= damage;

  if (entity->health <= 0)
  {
    entity->health = 0;
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
  return ref.entity->id == ref.id ? ref.entity : NULL;
}

// @EntityList =================================================================================

Entity *alloc_entity(Game *game)
{
  EntityList *entities = &game->entities;
  Entity *new_entity = game->entities.first_free;

  if (new_entity == NULL)
  {
    new_entity = arena_alloc(&game->arena, sizeof (Entity));
    clear_entity(new_entity);

    if (entities->head == NULL)
    {
      entities->head = new_entity;
    }

    if (entities->tail != NULL)
    {
      entities->tail->next = new_entity;
    }

    new_entity->next = NULL;
    entities->tail = new_entity;
    entities->count++;
  }
  else
  {
    entities->first_free = entities->first_free->next_free;
  }

  new_entity->id = random_u64(1, UINT64_MAX);

  return new_entity;
}

void free_entity(Game *game, Entity *entity)
{
  EntityList *entities = &game->entities;

  clear_entity(entity);
  entity->next_free = entities->first_free;
  entities->first_free = entity;
}

#ifdef DEBUG
static
void print_lists(Game *game)
{
  EntityList list = game->entities;

  printf("Arena: ");

  for (Entity *e = list.head; e != NULL; e = e->next)
  {
    printf("%llu -> ", e->id);
  }
  
  printf("NULL\n");

  printf("Freelist: ");

  Entity *curr = list.first_free;
  while (curr)
  {
    printf("%llu -> ", curr->id);
    curr = curr->next_free;
  }

  printf("NULL\n");
  printf("\n");
}
#endif

Entity *get_entity_of_id(Game *game, u64 id)
{
  Entity *result = NULL;
  
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
  Entity *result = NULL;
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

// @Collider2D =================================================================================



// @Timer ======================================================================================

static
void init_timers(Entity *entity)
{
  entity->timers[TIMER_COMBAT] = (Timer)
  {
    .start_duration = 1.0f,
    .curr_duration = 1.0f,
    .should_tick = TRUE,
    .is_ticking = FALSE,
    .timeout = FALSE,
    .should_loop = TRUE
  };

  entity->timers[TIMER_HEALTH] = (Timer)
  {
    .start_duration = 1.0f,
    .curr_duration = 1.0f,
    .should_tick = TRUE,
    .is_ticking = FALSE,
    .timeout = FALSE,
    .should_loop = FALSE
  };

  entity->timers[TIMER_KILL] = (Timer)
  {
    .start_duration = 5.0f,
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
  if (timer->should_tick)
  {
    if (timer->is_ticking)
    {
      timer->curr_duration -= dt;

      if (timer->curr_duration <= 0.0f)
      {
        timer->timeout = TRUE;
        timer->is_ticking = FALSE;
        timer->should_tick = timer->should_loop ? TRUE : FALSE;
      }
    }
    else
    {
      timer->curr_duration = timer->start_duration;
      timer->is_ticking = TRUE;
      timer->timeout = FALSE;
    }
  }

  return timer->timeout;
}
