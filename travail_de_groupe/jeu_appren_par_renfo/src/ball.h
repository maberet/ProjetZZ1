#ifndef BALL_H
#define BALL_H

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h> 
#include "render.h"



#include "render.h"

#define NUMBERPOINT_TRAJEC 50

#define PLAYER 0
#define AI 1
#define NONE 2

typedef enum
{
    false, true
    
} booleen_t;

typedef struct ball
{
    float x;
    float y;
    float z;
    int h;
    int w;
    int isHit;
    float angle;
    int speed;
    int isTravelingTo;
} ball_t;

extern ball_t ball;
typedef struct point{
    int x;
    int y; 
} point_t ; 

//extern ball_t ball;
extern int trajectoireAntoine[NUMBERPOINT_TRAJEC][2];
extern int whoScored;

void initBall();
float defineAngle(int, int, int, int);
void updateBall();
point_t  initCanon (point_t canon);
point_t initDropPoint (point_t dropPoint);
float defineAngleF(int canonX, int canonY, int xDropPoint, int yDropPoint);
float defineAngleH(int xCanon, int xDropPoint);

float lagrangeInterpolation(float, int, int, int, int, int, int);
void calculTrajectoireAntoine2(int, int, int, int, int, int);

#endif