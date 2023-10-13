#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "base/base_common.h"
#include "base/base_math.h"
#include "util.h"
#include "entity.h"

extern Input *input;

static
Vec2F random_position(u32 min_x, u32 max_x, u32 min_y, u32 max_y);

Entity entity_create(EntityType type)
{
  Entity entity = {0};
  entity.type = type;
  entity.move_state = EntityState_Idle;
  entity.scale = v2f(2.0f, 2.0f);
  entity.rotation = 45.0f;
  // entity.rotation = 0.0f;
  entity.width = 10.0f * entity.scale.x;
  entity.height = 10.0f * entity.scale.y;
  entity.active = TRUE;
  entity.hurt_cooldown.max_duration = 1.0f;

  switch (type)
  {
    case EntityType_Player:
    {
      entity.color = v4f(0.9f, 0.9f, 0.9f, 1.0f);
      entity.speed = PLAYER_SPEED;
      entity.health = PLAYER_HEALTH;
      break;
    }
    case EntityType_Enemy:
    {
      entity.color = v4f(0.8f, 0.3f, 0.2f, 1.0f);
      entity.speed = 100.0f;
      entity.view_dist = 250;
      break;
    }
    default: break;
  }

  return entity;
}

void entity_start(Entity *entity)
{
  switch (entity->type)
  {
    case EntityType_Player:
    {
      // entity->pos = v2f(W_WIDTH/2.0f, W_HEIGHT/2.0f);
      entity->pos = V2F_ZERO;
      break;
    }
    case EntityType_Enemy:
    {
      entity->pos = random_position(0, W_WIDTH, 0, W_HEIGHT);
      break;
    }
    default: break;
  }
}

void entity_update(Entity *entity, f64 dt)
{
  if (!entity->active)
  {
    entity->dir = V2F_ZERO;
    return;
  }

  switch (entity->type)
  {
    case EntityType_Player:
    {
      if (input->a) entity->rotation += 120.0f * dt;
      if (input->d) entity->rotation -= 120.0f * dt;

      if (input->space)
      {
        entity->speed = lerp_1f(entity->speed, PLAYER_SPEED * 3.0f, PLAYER_ACC * dt);
      }
      else 
      {
        entity->speed = lerp_1f(entity->speed, PLAYER_SPEED, PLAYER_FRIC * dt);
      }

      if ((input->a && input->d) || (!input->a && !input->d))
      {
        entity->dir.x = 0.0f;
      }

      entity->vel.x = cosf(entity->rotation * RADIANS) * entity->speed * dt;
      entity->vel.y = sinf(entity->rotation * RADIANS) * entity->speed * dt;
    }
    break;
    case EntityType_Enemy:
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
    break;
    default: break;
  }

  // if (entity->dir.x != 0.0f || entity->dir.y != 0.0f)
  // {
  //   entity->dir = normalize_2f(entity->dir);
  // }

  // // X Acceleration
  // if (entity->dir.x != 0.0f)
  // {
  //   entity->vel.x += PLAYER_ACC * dir(entity->dir.x) * dt;
  //   entity->vel.x = clamp(
  //                         entity->vel.x, 
  //                        -entity->speed * abs(entity->dir.x) * dt,
  //                         entity->speed * abs(entity->dir.x) * dt);
  // }
  // else
  // {
  //   entity->vel.x = lerp_1f(entity->vel.x, 0.0f, PLAYER_FRIC * dt);
  //   entity->vel.x = to_zero(entity->vel.x, 0.1f);
  // }

  // // Y Acceleration
  // if (entity->dir.y != 0.0f)
  // {
  //   entity->vel.y += PLAYER_ACC * dir(entity->dir.y) * dt;
  //   entity->vel.y = clamp(
  //                         entity->vel.y, 
  //                        -entity->speed * abs(entity->dir.y) * dt, 
  //                         entity->speed * abs(entity->dir.y) * dt);
  // }
  // else 
  // {
  //   entity->vel.y = lerp_1f(entity->vel.y, 0.0f, PLAYER_FRIC * dt);
  //   entity->vel.y = to_zero(entity->vel.y, 0.1f);
  // }

  entity->pos = add_2f(entity->pos, entity->vel);

  Mat3x3F xform = m3x3f(1.0f);
  xform = mul_3x3f(scale_3x3f(entity->scale.x, entity->scale.y), xform);
  xform = mul_3x3f(rotate_3x3f(entity->rotation), xform);
  xform = mul_3x3f(translate_3x3f(entity->pos.x, entity->pos.y), xform);
  xform = mul_3x3f(translate_3x3f(entity->width/2, entity->height/2), xform);
  entity->xform = xform;

  // printf("Pos: %f, %f\n", entity->pos.x, entity->pos.y);
  // printf("Dir: %f, %f\n", entity->dir.x, entity->dir.y);
  // printf("Vel: %f, %f\n", entity->vel.x, entity->vel.y);
}

void entity_set_target(Entity *entity, Vec2F target_pos)
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

void entity_deal_damage(Entity *target)
{
  target->health -= 1;

  if (target->health <= 0)
  {
    target->health = 0;
    target->active = FALSE;
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
