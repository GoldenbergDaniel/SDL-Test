#include <SDL2/SDL.h>

#include "globals.h"
#include "util.h"
#include "draw.h"

void clear_background(SDL_Renderer *renderer, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
}

void draw_rect(SDL_Renderer *renderer, v2f pos, i32 w, i32 h, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRectF(renderer, &((SDL_FRect) {pos.x, pos.y, w, h}));
}

void draw_rect_outline(SDL_Renderer *renderer, v2f pos, i32 w, i32 h, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawRectF(renderer, &((SDL_FRect) {pos.x, pos.y, w, h}));
}
