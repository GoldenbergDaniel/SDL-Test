#pragma once

typedef struct Enemy
{
    u16 width;
    u16 height;
    SDL_Color color;
    v2 pos;
    v2 vel;
    v2 dir;
    f32 speed;
    bool has_target;
    v2 target_pos;
    f32 target_angle;
    u8 col_mask;
    u16 view_dist;
} Enemy;

Enemy create_enemy(u16 width, u16 height, SDL_Color color);
void init_enemy(Enemy *enemy);
void update_enemy(Enemy *enemy, f64 t, f64 dt);
void enemy_find_target(Enemy *enemy, v2 *target_pos);
