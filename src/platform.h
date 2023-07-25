#pragma once

#include <SDL2/SDL.h>

#include "common.h"
#include "util.h"

void clear_background(SDL_Renderer *renderer, Vec4U8 c);
void draw_rect(SDL_Renderer *renderer, Vec2F32 pos, f32 w, f32 h, Vec4U8 c);
void draw_rect_outline(SDL_Renderer *renderer, Vec2F32 pos, f32 w, f32 h, Vec4U8 c);
