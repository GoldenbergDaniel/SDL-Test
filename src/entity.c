#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "base/base_common.h"
#include "base/base_math.h"
#include "game.h"
#include "util.h"
#include "entity.h"

extern Global *GLOBAL;

static
Vec2F random_position(u32 min_x, u32 max_x, u32 min_y, u32 max_y);

Entity create_player_entity(void)
{
  Entity entity = {0};
  entity.type = EntityType_Ship;
  entity.state = EntityState_Idle;
  entity.flags = EntityFlag_Player | EntityFlag_Movable;
  entity.pos = v2f(W_WIDTH / 2.0f, 0.0f);
  entity.scale = v2f(1.0f, 1.0f);
  entity.rot = 0.0f;
  entity.width = 20.0f * entity.scale.x;
  entity.height = 20.0f * entity.scale.y;
  entity.color = v4f(0.9f, 0.9f, 0.9f, 1.0f);
  entity.speed = PLAYER_SPEED;
  entity.health = PLAYER_HEALTH;
  entity.active = TRUE;

  return entity;
}

Entity create_enemy_entity(EntityType type)
{
  Entity entity = {0};
  entity.type = type;
  entity.state = EntityState_Idle;
  entity.flags = EntityFlag_Enemy | EntityFlag_Movable;
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
  Input *input = GLOBAL->input;
  u64 flags = entity->flags;

  if (flags & EntityFlag_Player)
  {
    if (input->a) entity->rot += 200.0f * dt;
    if (input->d) entity->rot -= 200.0f * dt;

    if (input->w && !input->s)
    {
      entity->speed = lerp_1f(entity->speed, PLAYER_SPEED * 4.0f, PLAYER_ACC * dt);
      entity->state = EntityState_Move;
    }
    else if (!input->w && input->s)
    {
      entity->speed = lerp_1f(entity->speed, PLAYER_SPEED * 0.5f, PLAYER_FRIC * dt);
      entity->state = EntityState_Move;
    }
    else 
    {
      entity->speed = lerp_1f(entity->speed, PLAYER_SPEED, PLAYER_FRIC * dt);
      entity->state = EntityState_Slide;
    }

    entity->vel.x = cosf(entity->rot * RADIANS) * entity->speed * dt;
    entity->vel.y = sinf(entity->rot * RADIANS) * entity->speed * dt;
  }

  if (flags & EntityFlag_Enemy)
  {
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
      entity->state = EntityState_Move;
    }
    else
    {
      entity->vel.x = lerp_1f(entity->vel.x, 0.0f, PLAYER_FRIC * dt);
      entity->vel.x = to_zero(entity->vel.x, 0.1f);
      entity->state = EntityState_Slide;
    }

    // Y Acceleration
    if (entity->dir.y != 0.0f)
    {
      entity->vel.y += PLAYER_ACC * dir(entity->dir.y) * dt;
      entity->vel.y = clamp(
                            entity->vel.y, 
                           -entity->speed * abs(entity->dir.y) * dt, 
                            entity->speed * abs(entity->dir.y) * dt);
      entity->state = EntityState_Move;
    }
    else 
    {
      entity->vel.y = lerp_1f(entity->vel.y, 0.0f, PLAYER_FRIC * dt);
      entity->vel.y = to_zero(entity->vel.y, 0.1f);
      entity->state = EntityState_Slide;
    }
  }

  if (roundf(entity->vel.x) == 0.0f && roundf(entity->vel.y) == 0.0f)
  {
    entity->state = EntityState_Idle;
  }

  entity->pos = add_2f(entity->pos, entity->vel);

  Mat3x3F xform = m3x3f(1.0f);
  xform = mul_3x3f(scale_3x3f(entity->scale.x, entity->scale.y), xform);
  xform = mul_3x3f(rotate_3x3f(entity->rot * RADIANS), xform);
  xform = mul_3x3f(translate_3x3f(entity->pos.x, entity->pos.y), xform);
  // xform = mul_3x3f(translate_3x3f(entity->width/2, entity->height/2), xform);
  entity->xform = xform;
}

void set_entity_target(Entity *entity, Vec2F target_pos)
{
  if (distance_2f(entity->pos, target_pos) <= entity->view_dist)
  {
    entity->has_target = TRUE;
    f32 dist_x = target_pos.x - entity->pos.x;
    f32 dist_y = target_pos.y - entity->pos.y;
    entity->target_pos = target_pos;
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
