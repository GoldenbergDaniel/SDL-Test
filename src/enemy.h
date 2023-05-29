#pragma once
#define enemy_H

typedef struct Enemy
{
    u16 width;
    u16 height;
    SDL_Color color;
    v2 pos;
    v2 vel;
    v2 dir;
    f32 speed;
} Enemy;

Enemy create_enemy(u16 width, u16 height, SDL_Color color);
void init_enemy(Enemy *enemy);
void update_enemy(Enemy *enemy, f64 t, f64 dt);
