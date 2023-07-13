#pragma once

void clear_background(SDL_Renderer *renderer, Color8I c);
void draw_rect(SDL_Renderer *renderer, Vec2F pos, f32 w, f32 h, Color8I c);
void draw_rect_outline(SDL_Renderer *renderer, Vec2F pos, f32 w, f32 h, Color8I c);
