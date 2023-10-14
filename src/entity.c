#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "base/base_common.h"
#include "base/base_math.h"
#include "game.h"
#include "util.h"
#include "entity.h"

extern State *state;

static
Vec2F random_position(u32 min_x, u32 max_x, u32 min_y, u32 max_y);

Entity create_player_entity(void)
{
  Entity entity = {0};
  entity.type = EntityType_Ship;
  entity.state = EntityState_Idle;
  entity.flags = EntityFlag_Movable;
  entity.pos = v2f(W_WIDTH/2.0f, 0.0f);
  entity.scale = v2f(1.0f, 1.0f);
  entity.rot = 0.0f;
  entity.width = 20.0f * entity.scale.x;
  entity.height = 20.0f * entity.scale.y;
  entity.color = v4f(0.9f, 0.9f, 0.9f, 1.0f);
  entity.speed = PLAYER_SPEED;
  entity.health = PLAYER_HEALTH;
  entity.player = TRUE;
  entity.active = TRUE;

  return entity;
}

Entity create_enemy_entity(EntityType type)
{
  Entity entity = {0};
  entity.type = type;
  entity.state = EntityState_Idle;
  entity.flags = EntityFlag_Movable | EntityFlag_Enemy;
  entity.pos = random_position(0, W_WIDTH, 0, W_HEIGHT);
  entity.scale = v2f(2.0f, 2.0f);
  entity.rot = 0.0f;
  entity.width = 20.0f * entity.scale.x;
  entity.height = 20.0f * entity.scale.y;
  entity.color = v4f(9.0f, 0.2f, 0.1f, 1.0f);
  entity.speed = 100.0f;
  entity.view_dist = 250;
  entity.active = TRUE;

  return entity;
}

void update_entity_movement(Entity *entity, f64 dt)
{
  Input *input = state->input;
  u64 flags = entity->flags;

  if (entity->player)
  {
    if (input->a) entity->rot += 120.0f * dt;
    if (input->d) entity->rot -= 120.0f * dt;

    if (input->w && !input->s)
    {
      entity->speed = lerp_1f(entity->speed, PLAYER_SPEED * 4.0f, PLAYER_ACC * dt);
    }
    else if (!input->w && input->s)
    {
      entity->speed = lerp_1f(entity->speed, PLAYER_SPEED, PLAYER_FRIC * 1.5f * dt);
    }
    else 
    {
      entity->speed = lerp_1f(entity->speed, PLAYER_SPEED, PLAYER_FRIC * dt);
    }

    entity->vel.x = cosf(entity->rot * RADIANS) * entity->speed * dt;
    entity->vel.y = sinf(entity->rot * RADIANS) * entity->speed * dt;

    if (input->a) entity->rot += 100.0f * dt;
    if (input->d) entity->rot -= 100.0f * dt;
  }

  if (flags & EntityFlag_Enemy)
  {
    if (entity->has_target && entity->active)
    {
      entity->dir.x = sinf(entity->target_angle);
      entity->dir.y = cosf(entity->target_angle);
    }
    else
    {
      entity->dir = V2F_ZERO;
    }
  }

  entity->pos = add_2f(entity->pos, entity->vel);

  Mat3x3F xform = m3x3f(1.0f);
  xform = mul_3x3f(scale_3x3f(entity->scale.x, entity->scale.y), xform);
  xform = mul_3x3f(rotate_3x3f(entity->rot), xform);
  xform = mul_3x3f(translate_3x3f(entity->pos.x, entity->pos.y), xform);
  xform = mul_3x3f(translate_3x3f(entity->width/2, entity->height/2), xform);
  entity->xform = xform;

  // printf("Pos: %f, %f\n", entity->pos.x, entity->pos.y);
  // printf("Dir: %f, %f\n", entity->dir.x, entity->dir.y);
  // printf("Vel: %f, %f\n", entity->vel.x, entity->vel.y);
}

void set_entity_target(Entity *entity, Vec2F target_pos)
{
  if (distance_2f(entity->pos, target_pos) <= entity->view_dist)
  {
    entity->has_target = TRUE;
    entity->target_pos = target_pos;
    f32 dist_x = entity->target_pos.x - entity->pos.x;
    f32 dist_y = entity->target_pos.y - entity->pos.y;
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

void hurt_entity(Entity *entity)
{
  entity->health -= 1;

  if (entity->health <= 0)
  {
    entity->health = 0;
    entity->active = FALSE;
    printf("Player ded\n");
  }
}

static inline
Vec2F random_position(u32 min_x, u32 max_x, u32 min_y, u32 max_y)
{
  return (Vec2F)
  {
    (rand() % max_x) + min_x,
    (rand() % max_y) + min_y
  };
}
