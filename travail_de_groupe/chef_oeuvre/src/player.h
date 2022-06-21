#ifndef PLAYER_H
#define PLAYER_H

#include "main.h"
#include "gest_event.h"

typedef struct Player{
    int x;
    int y;
    int water_level;
} Player_t;

extern Player_t player;

void initPlayer();
void GestMovement();

#endif