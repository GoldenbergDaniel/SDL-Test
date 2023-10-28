#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "base/base_common.h"
#include "base/base_math.h"

#include "global.h"
#include "event.h"
#include "entity.h"
#include "game.h"

extern Global *GLOBAL;

static u64 random_u64(u64 min, u64 max);
static Vec2F random_position(u32 min_x, u32 max_x, u32 min_y, u32 max_y);
static void init_timers(Entity *entity);
static void timer_start(Timer *timer, bool start_at_zero);
static bool timer_tick(Timer *timer, f64 dt);
static void print_lists(Game *game);

// @Entity =====================================================================================

void init_player_entity(Entity *entity)
{
  entity->type = EntityType_Player;
  entity->props = PLAYER_PROPS;
  entity->pos = v2f(W_WIDTH / 2.0f, W_HEIGHT / 2.0f);
  entity->scale = v2f(1.0f, 1.0f);
  entity->speed = PLAYER_SPEED;
  entity->width = 20.0f * entity->scale.x;
  entity->height = 20.0f * entity->scale.y;
  entity->color = COLOR_WHITE;
  entity->active = TRUE;
  entity->visible = TRUE;

  init_timers(entity);
}

void init_enemy_entity(Entity *entity)
{
  entity->type = EntityType_EnemyShip;
  entity->props = ENEMY_PROPS;
  entity->pos = random_position(0, W_WIDTH, 0, W_HEIGHT);
  entity->scale = v2f(2.0f, 2.0f);
  entity->speed = 100.0f;
  entity->width = 20.0f * entity->scale.x;
  entity->height = 20.0f * entity->scale.y;
  entity->color = COLOR_RED;
  entity->view_dist = 250;
  entity->active = TRUE;
  entity->visible = TRUE;

  init_timers(entity);
}

void init_laser_entity(Entity *entity)
{
  entity->type = EntityType_Laser;
  entity->props = LASER_PROPS;
  entity->pos = V2F_ZERO;
  entity->vel = V2F_ZERO;
  entity->scale = v2f(3.0f, 0.15f);
  entity->rot = 0.0f;
  entity->speed = 0.0f;
  entity->width = 20.0f * entity->scale.x;
  entity->height = 20.0f * entity->scale.y;
  entity->color = COLOR_GREEN;
  entity->active = TRUE;
  entity->visible = TRUE;

  init_timers(entity);
}

void clear_entity(Entity *entity)
{
  Entity *next = entity->next;
  *entity = (Entity) {0};
  entity->next = next;
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
  Timer *timer = &entity->timers[TIMER_KILL];

  if (timer->ticking)
  {
    timer_tick(timer, game->dt);

    if (timer->timeout)
    {
      EventDesc desc = 
      {
        .id = entity->id
      };

      push_event(game, EventType_KillEntity, desc);
    }
  }
  else
  {
    timer_start(timer, FALSE);
  }

  entity->vel.x = cosf(entity->rot * RADIANS) * entity->speed * game->dt;
  entity->vel.y = sinf(entity->rot * RADIANS) * entity->speed * game->dt;
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
    entity->speed = lerp_1f(entity->speed, PLAYER_SPEED * 4.0f, PLAYER_ACC * dt);
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

  if (entity->has_target && entity->active)
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
  if (key_just_pressed(KEY_SPACE))
  {
    Vec2F shot_point = v2f(entity->pos.x, entity->pos.y);
    EventDesc desc = 
    {
      .type = EntityType_Laser,
      .position = shot_point,
      .rotation = entity->rot,
      .speed = 1000.0f,
    };

    push_event(game, EventType_SpawnEntity, desc);
  }
}

void update_targetting_entity_combat(Game *game, Entity *entity)
{
  // Do combat
}

void set_entity_target(Entity *entity, EntityRef target)
{
  Entity *target_entity = entity_from_ref(target);
  
  if (target_entity == NULL) return;

  Vec2F target_pos = target_entity->pos;

  if (distance_2f(entity->pos, target_pos) <= entity->view_dist)
  {
    entity->has_target = TRUE;
    f32 dist_x = target_pos.x - entity->pos.x;
    f32 dist_y = target_pos.y - entity->pos.y;
    entity->target_angle = atan2(dist_x, dist_y);
  }
  else
  {
    entity->has_target = FALSE;
    entity->target_pos = V2F_ZERO;
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
    entity->active = FALSE;
    entity->visible = FALSE;
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

// #ifdef DEBUG
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
// #endif

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

// @Timer ======================================================================================

static
void init_timers(Entity *entity)
{
  entity->timers[TIMER_COMBAT] = (Timer)
  {
    .start_duration = 1.0f,
    .curr_duration = 1.0f,
    .should_tick = FALSE,
    .ticking = FALSE,
    .timeout = FALSE,
    .loop = TRUE
  };

  entity->timers[TIMER_HEALTH] = (Timer)
  {
    .start_duration = 1.0f,
    .curr_duration = 1.0f,
    .should_tick = FALSE,
    .ticking = FALSE,
    .timeout = FALSE,
    .loop = FALSE
  };

  entity->timers[TIMER_KILL] = (Timer)
  {
    .start_duration = 5.0f,
    .curr_duration = 5.0f,
    .should_tick = FALSE,
    .ticking = FALSE,
    .timeout = FALSE,
    .loop = FALSE
  };
}

static
void timer_start(Timer *timer, bool start_at_zero)
{
  timer->curr_duration = start_at_zero ? 0.0f : timer->start_duration;
  timer->ticking = TRUE;
  timer->timeout = FALSE;
  timer->should_tick = TRUE;
}

static
bool timer_tick(Timer *timer, f64 dt)
{
  if (timer->should_tick)
  {
    timer->curr_duration -= dt;

    if (timer->curr_duration <= 0.0f)
    {
      timer->timeout = TRUE;
      timer->ticking = FALSE;
    }
  }

  return timer->timeout;
}

static inline
u64 random_u64(u64 min, u64 max)
{
  return rand() % max + min;
}

static inline
Vec2F random_position(u32 min_x, u32 max_x, u32 min_y, u32 max_y)
{
  return (Vec2F)
  {
    rand() % max_x + min_x,
    rand() % max_y + min_y
  };
}
