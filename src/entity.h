#pragma once

typedef enum EntityType
{
    ENTITY_PLAYER,
    ENTITY_ENEMY
} EntityType;

typedef struct Entity
{
    EntityType type;
    u16 width;
    u16 height;
    v2f pos;
    v2f vel;
    v2f dir;
    f32 speed;
    bool has_target;
    v2f target_pos;
    f32 target_angle;
    u8 col_mask;
    u16 view_dist;
    SDL_Color color;
} Entity;

Entity entity_create(EntityType type);
void entity_init(Entity *entity);
void entity_update(Entity *entity, f64 t, f64 dt);
void entity_set_target(Entity *entity, v2f target_pos);
