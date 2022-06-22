#ifndef PLAYER_H
#define PLAYER_H

#include "main.h"
#include "gest_event.h"

typedef struct player{
    int x;
    int y;
    int waterLevel;
} player_t;

extern player_t player;

void initPlayer();
void manageMovement();

#endif