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
    int w;
    int h;
    int currentWater;
    int waterMax;
    int speed;
    int isMoving;
    int direction;
    int HPMax;
    int currentHP;
    int invisible;
    int invisibleTime;
    int timeLastHitFire;
    int score;
} player_t;

extern player_t player;

void initPlayer();
void manageMovement();
int selectStateHover();
void pourWater(int, int);
void fillWater(int, int);
int giveCaseBelowPosition(int, int);
void updatePlayer();
void initAll();

#endif