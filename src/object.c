#include "globals.h"
#include "util.h"
#include "draw.h"
#include "object.h"

v2 get_center(u16 width, u16 height)
{
    return (v2) { 
        WINDOW_WIDTH/2.0f - width/2.0f,
        WINDOW_HEIGHT/2.0f - height/2.0f,
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

    // object->vel.x = 200.0f;
}

void update_object(Object *object, f64 t, f64 dt)
{
    object->pos.x += lerp(object->pos.x, WINDOW_WIDTH-object->width, 0.7f) * dt;
    // object->pos.y += lerp(object->pos.y, WINDOW_HEIGHT-object->height, 0.7f);

    // object->pos.x += cosf(t*2) * 100 * dt;
    // object->pos.y += sinf(t*2) * 100 * dt;
    
    object->vel.x += object->acc.x;
    object->vel.y += object->acc.y;

    object->pos.x += object->vel.x * dt;
    object->pos.y += object->vel.y * dt;
}

void draw_object(Object *object, SDL_Renderer *renderer)
{
    draw_rect(renderer, object->pos, object->width, object->height, object->color);
}
