#ifndef _RENDER_H_
#define _RENDER_H_

#include "gest_event.h"
#include "ball.h"
#include "canon.h"

void createWindow();
void endSDL();
void drawString(char *, int, int, int, int, int, int, int, int);
void drawTerrainTopView();
void initPointDeChute();
void newCanon();
//tofinish
void mainLoop();

#endif 