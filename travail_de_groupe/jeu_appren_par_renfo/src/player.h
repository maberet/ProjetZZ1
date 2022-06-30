#ifndef PLAYER_H
#define PLAYER_H

#include "map.h"
#include "render.h"
#include "ball.h"

#define ENTITIES_UP 0
#define ENTITIES_DOWN 1
#define ENTITIES_LEFT 2
#define ENTITIES_RIGHT 3

#define HIT_RANGE 2
#define HIT_FORCE 2
#define MOVEMENT_SPEED 10
#define MAX_HIT_TIME 3

typedef struct player
{
    float x;
    float y;
    float z;
    int h;
    int w;
    int speed;
    int isMoving;
    int startHitBool;
    int isHit;
    int direction;
    float angle;
    float deltax;
    float deltay;
    float viewAngle;
    float startHitTimer;
    float endHitTimer;
    float hitIntensity;
    float hitIntensityTimer;
    int isHoldingClick;
} player_t;

extern player_t player;
extern player_t ennemy;
extern int landingPointPlayerX;
extern int landingPointPlayerY;

extern int lastHitPointPlayerX;
extern int lastHitPointPlayerY;
extern int landingPointPlayerIsFind;

extern int rxWall;

int generatelandingPointPlayer(int rxWall, float hitIntensity);
void initPlayer();
void managePlayer();
void freeIntList(int *list);

#endif