#ifndef PLAYER_H
#define PLAYER_H

#include "main.h"

typedef struct Player{
    int x;
    int y;
    int water_level;
} Player_t;

extern Player_t player;

void initPlayer();

#endif