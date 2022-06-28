#ifndef _RENDER_H_
#define _RENDER_H_

#include "main.h"
#include "gest_event.h"
#include "map.h"
#include "player.h"

#define FPS_TO_GET 60
#define CELL_SIZE 10
 
#define DOF 100
#define BLOCK_SIZE 64
#define DR 0.0174533
#define FOV_ANGLE 120
#define pi 3.14159265358979323846
#define NB_RAYS (screenDimension.w)
#define ANGLE_INC ((DR * FOV_ANGLE) / NB_RAYS)

typedef struct rayInfo{
    float ra;
    float distT;
    int r;
    int isTransparent;
    int direction; 
    float htexture;
    struct rayInfo * next;
} rayInfo_t;

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_DisplayMode screenDimension;


void mainLoop();

#endif 