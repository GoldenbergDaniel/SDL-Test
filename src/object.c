#include "globals.h"
#include "util.h"
#include "draw.h"
#include "object.h"
#include "input.h"

#include <stdio.h>
#include <math.h>

extern Input *input;

v2 dir;

v2 get_center(u16 width, u16 height)
{
    return (v2) { 
        WINDOW_WIDTH/2.0f - width/2.0f,
        WINDOW_HEIGHT/2.0f - height/2.0f
    };
}

Object new_object(u16 width, u16 height, SDL_Color color) 
{
    return (Object) {
        width,
        height,
        color
    };
}

void init_object(Object *object)
{
    object->pos = (v2) {
        get_center(object->width, object->height).x,
        get_center(object->width, object->height).y
    };
}

void update_object(Object *object, f64 t, f64 dt)
{
    // object->pos.x += lerp(object->pos.x, WINDOW_WIDTH-object->width, 0.7f) * dt;
    // object->pos.y += lerp(object->pos.y, WINDOW_HEIGHT-object->height, 0.7f) * dt;

    i32 speed = 500.0f;
    
    if (input->d)
        dir.x = 1.0f;

    if (input->a)
        dir.x = -1.0f;

    if (input->w)
        dir.y = -1.0f;

    if (input->s)
        dir.y = 1.0f;

    if (!input->a && !input->d) 
    {
        object->vel.x = 0.0f;
        dir.x = 0.0f;
    }

    if (!input->w && !input->s)
    {
        object->vel.y = 0.0f;
        dir.y = 0.0f;
    }

    object->vel.x = speed * dir.x;
    object->vel.y = speed * dir.y;

    f32 magnitude = sqrtf(powf(object->vel.x, 2) + powf(object->vel.y, 2));

    if (magnitude > 0.0f)
    {
        object->vel.x = (object->vel.x/magnitude) * speed;
        object->vel.y = (object->vel.y/magnitude) * speed;
    }
    
    object->pos.x += object->vel.x * dt;
    object->pos.y += object->vel.y * dt;
}

void draw_object(Object *object, SDL_Renderer *renderer)
{
    draw_rect(renderer, object->pos, object->width, object->height, object->color);
}
