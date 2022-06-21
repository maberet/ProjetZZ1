#ifndef _RENDER_H_
#define _RENDER_H_

#include "main.h"
#include "gest_event.h"
#include "player.h"

#define FPS_TO_GET 60
#define CELLSIZE (ScreenDimension.h / MAPSIZE)

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_DisplayMode ScreenDimension;

void drawHover(int x, int y);
void MainLoop();

#endif 