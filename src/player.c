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

void init_player(Player *object)
{
    object->pos = (v2) {
        get_center(object->width, object->height).x,
        get_center(object->width, object->height).y
    };

    object->speed = 300.0f;
}

void update_player(Player *object, f64 t, f64 dt)
{   
    if (input->a)
        object->dir.x = -1.0f;

    if (input->d)
        object->dir.x = 1.0f;

    if (input->w)
        object->dir.y = -1.0f;

    if (input->s)
        object->dir.y = 1.0f;

    if (!input->a && !input->d) 
        object->dir.x = 0.0f;

    if (!input->w && !input->s)
        object->dir.y = 0.0f;

    if (object->dir.x != 0.0f || object->dir.y != 0.0f)
        object->dir = normalize(object->dir);

    object->vel.x = object->speed * object->dir.x * dt;
    object->vel.y = object->speed * object->dir.y * dt;

    object->pos = add_vec(object->pos, object->vel);
}

void draw_player(Player *object, SDL_Renderer *renderer)
{
    draw_rect(renderer, object->pos, object->width, object->height, object->color);
}
