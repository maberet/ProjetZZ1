#ifndef Q_LEARN
#define Q_LEARN

#include <stdio.h>
#include <stdlib.h>
#include <time.h>   
//#include "ball.h"
#include "math.h"
#include "map.h"

#define NUMBER_ZONE_SHOOTER 4
#define NUMBER_ZONE_RECEIVER 4

#define FOWARD 0//<--
#define BACK 1 //-->
#define UP 2
#define DOWN 3
#define WAIT 4 

typedef struct agent {
    int x;
    int y;
    int heigth;
    int weight;
    int speed; 
} agent_t;


void moveAgent(agent_t * agent, int choice);
float ***** allocateAndInitiateQ();
void writeQ(float *****Q);
#endif