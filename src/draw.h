#pragma once

void clear_background(SDL_Renderer *renderer, SDL_Color color);
void draw_rect(SDL_Renderer *renderer, v2f pos, i32 w, i32 h, SDL_Color color);
void draw_rect_outline(SDL_Renderer *renderer, v2f pos, i32 w, i32 h, SDL_Color color);
