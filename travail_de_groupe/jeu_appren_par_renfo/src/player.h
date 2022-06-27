#ifndef PLAYER_H
#define PLAYER_H

#include "map.h"

#define ENTITIES_UP 0
#define ENTITIES_DOWN 1
#define ENTITIES_LEFT 2
#define ENTITIES_RIGHT 3

typedef struct player{
    int x;
    int y;
    int speed;
    int isMoving;
    int direction;
    float angle;
} player_t;

extern player_t player;

void initPlayer();

#endif