#ifndef _RENDER_H_
#define _RENDER_H_

#include <SDL2/SDL.h>

#include "main.h"
#include "event.h"

void initSDL2();
void afficherEcran();
void destroySDL2();

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Rect rect;
extern int width;
extern int height;

#endif 