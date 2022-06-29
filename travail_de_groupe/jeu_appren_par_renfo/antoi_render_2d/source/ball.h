#ifndef BALL_H
#define BALL_H

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include "canon.h"

#define NUMBERPOINT 200

typedef enum {
 false,true
}booleen_t; 

typedef struct  ball { 
    int x; 
    int y; 
    int z;
    float angle;
    float speed; 
}ball_t; 

typedef struct laGrange{
    float a;
    float b; 
    float c; 
}laGrange_t; 

extern ball_t ball;
extern laGrange_t coefLagrange; 
extern int trajectory[NUMBERPOINT][2]; 

extern int trajectoireAntoine[20][2];

void  initBall();
void laGrange(float *, float *);
float defineAngle(canon_t, int, int);
//void calculationTrajectory(canon_t, int, int, int, int);
void calculationTrajectory(canon_t, int, int);

int calculTrajectoireAntoine(float, int, int, int, int, int, int);
void calculTrajectoireAntoine2(int, int, int, int, int, int);

#endif 