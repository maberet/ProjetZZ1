#ifndef _GEST_EVENT_H_
#define _GEST_EVENT_H_


#include "main.h"
#include "render.h"

extern SDL_Point mousePosition;
extern int keys[10];

void *eventLoop(void *arg);

#endif 