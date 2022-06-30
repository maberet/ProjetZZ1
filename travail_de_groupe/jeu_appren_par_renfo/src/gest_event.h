#ifndef _GEST_EVENT_H_
#define _GEST_EVENT_H_

#include "main.h"
#include "render.h"

#define NB_KEYS 10

extern int Keys[NB_KEYS];

void *EventLoop(void *arg);
void initKeys();

#endif