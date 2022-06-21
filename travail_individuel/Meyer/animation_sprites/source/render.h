#ifndef _RENDER_H_
#define _RENDER_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "main.h"
#include "event.h"
#include "utility.h"

void initSDL2();
void afficherEcran();
void destroySDL2();
SDL_Texture* load_texture_from_image(char *, SDL_Window *, SDL_Renderer *);

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Texture *texture;
extern SDL_Rect my_rect;
extern int width;
extern int height;


#endif 