#pragma once

void clear_background(SDL_Renderer *renderer, ColorI8 c);
void draw_rect(SDL_Renderer *renderer, Vec2F pos, f32 w, f32 h, ColorI8 c);
void draw_rect_outline(SDL_Renderer *renderer, Vec2F pos, f32 w, f32 h, ColorI8 c);
