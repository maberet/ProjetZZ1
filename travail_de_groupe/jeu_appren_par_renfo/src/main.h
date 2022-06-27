#ifndef _MAIN_HEADER_
#define _MAIN_HEADER_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <pthread.h>

#include "render.h"
#include "gest_event.h"
#include "ball.h"


#define MENU 0
#define GAME 1

extern int running;
extern int game_state;


#endif 