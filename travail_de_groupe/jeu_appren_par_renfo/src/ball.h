#ifndef BALL_H
#define BALL_H

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

extern float coefLagrange[3]; 
#define NUMBERPOINT 5

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

void  initBall();
void laGrange (float beta[3],float Z[3]);
float defineAngle (canon_t ,int ,int );
void calculationTrajectory(canon_t ,float , int, int);

#endif 