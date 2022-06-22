#ifndef PLAYER_H
#define PLAYER_H

#include "main.h"
#include "gest_event.h"

#define PLAYER_UP 0
#define PLAYER_DOWN 1
#define PLAYER_LEFT 2
#define PLAYER_RIGHT 3

typedef struct player{
    int x;
    int y;
    int waterLevel;
    int speed;
} player_t;

extern player_t player;

void initPlayer();
void manageMovement();
int selectStateHover();

#endif