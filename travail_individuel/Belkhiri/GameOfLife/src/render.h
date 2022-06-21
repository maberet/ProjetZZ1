#ifndef _RENDER_H_
#define _RENDER_H_

#include "main.h"
#include "utility.h"
#include "gest_event.h"

#define FPS_TO_GET 1000/60.0
#define CELLSIZE (ScreenDimension.h/MAPSIZE)

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_DisplayMode ScreenDimension;


void MainLoop();

#endif