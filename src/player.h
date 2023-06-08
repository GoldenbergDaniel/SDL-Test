#pragma once

typedef struct Player
{
    u16 width;
    u16 height;
    SDL_Color color;
    v2f pos;
    v2f vel;
    v2f dir;
    f32 speed;
    u8 col_mask;
} Player;

Player create_player(u16 width, u16 height, SDL_Color color);
void init_player(Player *player);
void update_player(Player *player, f64 t, f64 dt);
