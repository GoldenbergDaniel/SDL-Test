#pragma once

void clear_background(SDL_Renderer *renderer, SDL_Color c);
void draw_rect(SDL_Renderer *renderer, v2f pos, f32 w, f32 h, SDL_Color c);
void draw_rect_outline(SDL_Renderer *renderer, v2f pos, f32 w, f32 h, SDL_Color c);
