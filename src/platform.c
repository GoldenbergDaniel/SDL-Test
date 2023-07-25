#include <SDL2/SDL.h>

#include "common.h"
#include "util.h"
#include "platform.h"

inline 
void clear_background(SDL_Renderer *renderer, Vec4U8 c)
{
  SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
  SDL_RenderClear(renderer);
}

inline 
void draw_rect(SDL_Renderer *renderer, Vec2F32 pos, f32 w, f32 h, Vec4U8 c)
{
  SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
  SDL_RenderFillRectF(renderer, &((SDL_FRect) {pos.x-w/2, pos.y-h/2, w, h}));
}

inline 
void draw_rect_outline(SDL_Renderer *renderer, Vec2F32 pos, f32 w, f32 h, Vec4U8 c)
{
  SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
  SDL_RenderDrawRectF(renderer, &((SDL_FRect) {pos.x-w/2, pos.y-h/2, w, h}));
}
