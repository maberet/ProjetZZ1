#ifndef _RENDER_H_
#define _RENDER_H_

#include "gest_event.h"
#include "ball.h"
#include "canon.h"

extern int zone_canon;
extern int zone_chute;
extern SDL_Rect terrain;

void createWindow();
void endSDL();
void drawString(char *, int, int, int, int, int, int, int, int);
void drawTerrainTopView();
void initPointDeChute();
void newCanon();
int getZoneChute(int, int, int, int);
void initTerrain();
//tofinish
void mainLoop();

#endif 