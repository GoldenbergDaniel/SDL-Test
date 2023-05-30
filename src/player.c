#include <stdio.h>
#include <SDL2/SDL.h>

#include "globals.h"
#include "util.h"
#include "draw.h"
#include "player.h"
#include "input.h"

extern Input *input;

Player create_player(u16 width, u16 height, SDL_Color color)
{
    return (Player) {
        width,
        height,
        color
    };
}

void init_player(Player *player)
{
    player->pos = (v2) {
        get_rect_center(player->width, player->height).x,
        get_rect_center(player->width, player->height).y
    };

    player->speed = 300.0f;
}

void update_player(Player *player, f64 t, f64 dt)
{   
    if (input->a) player->dir.x = -1.0f;
    if (input->d) player->dir.x = 1.0f;
    if (input->w) player->dir.y = -1.0f;
    if (input->s) player->dir.y = 1.0f;

    if ((!input->a && !input->d) || (input->a && input->d))
        player->dir.x = 0.0f;

    if ((!input->w && !input->s) || (input->w && input->s))
        player->dir.y = 0.0f;

    if (player->dir.x != 0.0f || player->dir.y != 0.0f)
        player->dir = normalize_v2(player->dir);

    player->vel.x = player->speed * player->dir.x * dt;
    player->vel.y = player->speed * player->dir.y * dt;

    player->pos = add_v2(player->pos, player->vel);
}
