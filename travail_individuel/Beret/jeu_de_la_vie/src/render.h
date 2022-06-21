#ifndef render_h   
#define render_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>

void init_sdl(SDL_Window *window_1 ,SDL_Renderer *renderer,int dimension_grille);
void dessiner_grille(SDL_Renderer* renderer, int ** grille, int taille);
void fin_sdl(SDL_Renderer* renderer,SDL_Window *window_1);
#endif 