#include <SDL2/SDL.h>

#include "globals.h"
#include "util.h"
#include "draw.h"

inline 
void clear_background(SDL_Renderer *renderer, SDL_Color c)
{
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    SDL_RenderClear(renderer);
}

inline 
void draw_rect(SDL_Renderer *renderer, v2f pos, i32 w, i32 h, SDL_Color c)
{
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    SDL_RenderFillRectF(renderer, &((SDL_FRect) {pos.x, pos.y, w, h}));
}

inline 
void draw_rect_outline(SDL_Renderer *renderer, v2f pos, i32 w, i32 h, SDL_Color c)
{
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    SDL_RenderDrawRectF(renderer, &((SDL_FRect) {pos.x, pos.y, w, h}));
}
