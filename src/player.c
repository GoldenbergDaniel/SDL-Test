#include "globals.h"
#include "util.h"
#include "draw.h"
#include "player.h"
#include "input.h"

#include <stdio.h>
#include <math.h>

extern Input *input;

v2 get_center(u16 width, u16 height)
{
    return (v2) {
        WINDOW_WIDTH/2.0f - width/2.0f,
        WINDOW_HEIGHT/2.0f - height/2.0f
    };
}

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
        get_center(player->width, player->height).x,
        get_center(player->width, player->height).y
    };

    player->speed = 300.0f;
}

void update_player(Player *player, f64 t, f64 dt)
{   
    if (input->a)
        player->dir.x = -1.0f;

    if (input->d)
        player->dir.x = 1.0f;

    if (input->w)
        player->dir.y = -1.0f;

    if (input->s)
        player->dir.y = 1.0f;

    if (!input->a && !input->d) 
        player->dir.x = 0.0f;

    if (!input->w && !input->s)
        player->dir.y = 0.0f;

    if (player->dir.x != 0.0f || player->dir.y != 0.0f)
        player->dir = normalize(player->dir);

    player->vel.x = player->speed * player->dir.x * dt;
    player->vel.y = player->speed * player->dir.y * dt;

    player->pos = add_vec(player->pos, player->vel);
}

void draw_player(Player *player, SDL_Renderer *renderer)
{
    draw_rect(renderer, player->pos, player->width, player->height, player->color);
}
