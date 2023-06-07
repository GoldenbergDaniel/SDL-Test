#include <math.h>
#include <SDL2/SDL.h>

#include "globals.h"
#include "util.h"
#include "draw.h"
#include "player.h"
#include "enemy.h"

Enemy create_enemy(u16 width, u16 height, SDL_Color color)
{
    return (Enemy) {
        width,
        height,
        color
    };
}

void init_enemy(Enemy *enemy)
{
    // TODO: Fix enemies spawning on invalid locations
    enemy->pos = get_random_position(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
    enemy->speed = 150.0f;
    enemy->view_dist = 1000;
    enemy->has_target = FALSE;
}

void update_enemy(Enemy *enemy, f64 t, f64 dt)
{   
    if (enemy->has_target)
    {
        enemy->dir.x = sinf(enemy->target_angle);
        enemy->dir.y = cosf(enemy->target_angle);
    }
    else
    {
        enemy->dir = V2_ZERO;
    }

    if (enemy->dir.x != 0.0f || enemy->dir.y != 0.0f)
    {
        enemy->dir = normalize_v2(enemy->dir);
    }

    enemy->vel.x = enemy->speed * enemy->dir.x * dt;
    enemy->vel.y = enemy->speed * enemy->dir.y * dt;

    enemy->pos = add_v2(enemy->pos, enemy->vel);
}

void enemy_find_target(Enemy *enemy, v2 *target_pos)
{
    // printf("TARGET POS X, Y: %f, %f\n", target_pos->x, target_pos->y);

    if (distance(enemy->pos, *target_pos) <= enemy->view_dist)
    {
        enemy->has_target = TRUE;
        enemy->target_pos = *target_pos;
        
        i16 dist_x = enemy->target_pos.x - enemy->pos.x;
        i16 dist_y = enemy->target_pos.y - enemy->pos.y;
        
        enemy->target_angle = atan2(dist_x, dist_y);
    }
    else
    {
        enemy->has_target = FALSE;
        enemy->target_pos = V2_ZERO;
    }
}
