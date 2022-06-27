#ifndef BALL_H
#define BALL_H

#include <stdlib.h>
#include <time.h>
#include <stdio.h>

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

extern ball_t monster;

void  initBall ();

#endif 