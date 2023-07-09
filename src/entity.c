#include <SDL2/SDL.h>

#include "globals.h"
#include "util.h"
#include "input.h"
#include "entity.h"

extern Input *input;

Entity entity_create(EntityType type)
{
    Entity e;
    e.type = type;

    switch (type)
    {
        case ENTITY_PLAYER:
        {
            e.width = 20;
            e.height = 20;
            e.color = COLOR_WHITE;
            break;
        }
        case ENTITY_ENEMY:
        {
            e.width = 40;
            e.height = 40;
            e.color = COLOR_RED;
            break;
        }
        default: break;
    }

    return e;
}

void entity_init(Entity *entity)
{
    entity->vel = V2F_ZERO;
    entity->dir = V2F_ZERO;

    switch (entity->type)
    {
        case ENTITY_PLAYER:
        {
            entity->pos = get_screen_center(entity->width, entity->height);
            entity->speed = 300.0f;
            break;
        }
        case ENTITY_ENEMY:
        {
            entity->pos = get_random_position(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
            entity->speed = 100.0f;
            entity->view_dist = 1000;
            break;
        }
        default: break;
    }
}

void entity_update(Entity *entity, f64 t, f64 dt)
{
    if (entity->type == ENTITY_PLAYER)
    {
        if (input->a)
            entity->dir.x = -1.0f;

        if (input->d)
            entity->dir.x = 1.0f;

        if (input->w)
            entity->dir.y = -1.0f;

        if (input->s)
            entity->dir.y = 1.0f;

        if ((!input->a && !input->d) || (input->a && input->d))
            entity->dir.x = 0.0f;

        if ((!input->w && !input->s) || (input->w && input->s))
            entity->dir.y = 0.0f;
    }
    else if (entity->type == ENTITY_ENEMY)
    {
        if (entity->has_target)
        {
            entity->dir.x = sinf(entity->target_angle);
            entity->dir.y = cosf(entity->target_angle);
        }
        else
        {
            entity->dir = V2F_ZERO;
        }
    }

    if (entity->dir.x != 0.0f || entity->dir.y != 0.0f)
        entity->dir = normalize_v2f(entity->dir);

    entity->vel.x = entity->speed * entity->dir.x * dt;
    entity->vel.y = entity->speed * entity->dir.y * dt;

    entity->pos = add_v2f(entity->pos, entity->vel);
}

void entity_set_target(Entity *entity, v2f target_pos)
{
    if (distance_v2f(entity->pos, target_pos) <= entity->view_dist)
    {
        entity->has_target = TRUE;
        entity->target_pos = target_pos;
        
        i16 dist_x = entity->target_pos.x - entity->pos.x;
        i16 dist_y = entity->target_pos.y - entity->pos.y;
        
        entity->target_angle = atan2(dist_x, dist_y);
    }
    else
    {
        entity->has_target = FALSE;
        entity->target_pos = V2F_ZERO;
    }
}
