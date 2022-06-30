#ifndef BALL_H
#define BALL_H

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

#include "render.h"

#define NUMBERPOINT_TRAJEC 50

typedef enum
{
    false,
    true
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
} ball_t;

extern ball_t ball;
extern int trajectoireAntoine[NUMBERPOINT_TRAJEC][2];

void initBall();
float defineAngle(int, int, int, int);
void updateBall();

float lagrangeInterpolation(float, int, int, int, int, int, int);
void calculTrajectoireAntoine2(int, int, int, int, int, int);

#endif