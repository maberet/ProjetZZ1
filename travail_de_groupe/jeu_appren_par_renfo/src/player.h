#ifndef PLAYER_H
#define PLAYER_H

#include "map.h"
#include "render.h"

#define ENTITIES_UP 0
#define ENTITIES_DOWN 1
#define ENTITIES_LEFT 2
#define ENTITIES_RIGHT 3

#define MOVEMENT_SPEED 2
typedef struct player{
    float x;
    float y;
    int speed;
    int isMoving;
    int direction;
    int HPMax;
    int currentHP;
    int coins;
    float angle;
    float deltax;
    float deltay;
    float viewAngle;
} player_t;

extern player_t player;

void initPlayer();
void managePlayer();

#endif