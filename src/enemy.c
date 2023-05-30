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
    enemy->pos = get_random_position(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);

    enemy->speed = 300.0f;
}

void update_enemy(Enemy *enemy, f64 t, f64 dt)
{   
    enemy->vel.x = enemy->speed * enemy->dir.x * dt;
    enemy->vel.y = enemy->speed * enemy->dir.y * dt;

    enemy->pos = add_v2(enemy->pos, enemy->vel);
}
