#ifndef _RENDER_H_
#define _RENDER_H_

#include "main.h"
#include "gest_event.h"
<<<<<<< HEAD

#define FPS_TO_GET 60
=======
#include "map.h"

#define FPS_TO_GET 60
#define CELL_SIZE 10
 
#define DOF 8
#define BLOCK_SIZE 64
#define DR 0.0174533
#define FOV_ANGLE 60
#define pi 3.14159265358979323846
#define NB_RAYS (screenDimension.x)
#define ANGLE_INC ((DR * FOV_ANGLE) / NB_RAYS)
>>>>>>> main

extern SDL_Window *window;
extern SDL_Renderer *renderer;


<<<<<<< HEAD
void MainLoop();
=======
void mainLoop();
>>>>>>> main

#endif 