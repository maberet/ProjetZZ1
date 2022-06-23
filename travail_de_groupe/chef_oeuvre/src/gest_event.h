#ifndef _GEST_EVENT_H_
#define _GEST_EVENT_H_


#include "main.h"
#include "render.h"

#define NBKEYS 10

extern SDL_Point mousePosition;
extern int keys[NBKEYS];

void *eventLoop(void *arg);

#endif 