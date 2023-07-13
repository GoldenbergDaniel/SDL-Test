#pragma once

typedef enum EntityType
{
  ENTITY_PLAYER,
  ENTITY_ENEMY
} EntityType;

typedef struct Entity Entity;
struct Entity
{
  // General
  EntityType type;
  Color8I color;
  f32 width;
  f32 height;

  // Physics
  Vec2F pos;
  Vec2F vel;
  Vec2F dir;
  f32 speed;
  u8 col_mask;

  // Targetting
  bool has_target;
  Vec2F target_pos;
  f32 target_angle;
  u16 view_dist;

  // Health
  i8 health;
  bool is_active;
  Timer hurt_cooldown;
};

#define PLAYER_HEALTH 3
#define PLAYER_SPEED 300.0f

Entity entity_create(EntityType type);
void entity_start(Entity *entity);
void entity_update(Entity *entity, f64 t, f64 dt);
void entity_set_target(Entity *entity, Vec2F target_pos);
void entity_deal_damage(Entity *target);
