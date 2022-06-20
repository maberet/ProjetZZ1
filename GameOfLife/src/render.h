#ifndef _RENDER_H_
#define _RENDER_H_

#include "main.h"
#include "utility.h"
#include "gest_event.h"

#define FPS_TO_GET 60

extern SDL_Window *window;
extern SDL_Renderer *renderer;


void MainLoop();

#endif