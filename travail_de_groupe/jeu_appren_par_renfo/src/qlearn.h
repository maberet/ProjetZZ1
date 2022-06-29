#ifndef Q_LEARN
#define Q_LEARN

#include <stdio.h>
#include <stdlib.h>
#include <time.h>   
//#include "ball.h"
#include "math.h"
#include "map.h"

#define M_PI 3.14159265358979323846

#define NUMBER_ZONE_SHOOTER 4
#define NUMBER_ZONE_RECEIVER 4
#define NUMBER_ACTION 5 

#define FOWARD 0//<--
#define BACK 1 //-->
#define UP 2
#define DOWN 3
#define WAIT 4 

typedef struct agent {
    int x;
    int y;
    int high;
    int weight;
    int speed; 
} agent_t;

typedef struct line {
    int receiverZone;
    int shooterZone; 
    int angleHZone;
    int angleFZone; 
    struct line *next ;

}line_t,*ptline_t; 

typedef struct point{
    int x;
    int y; 
} point_t ; 


agent_t* initAgent ( );
void moveAgent(agent_t * agent, int choice);
float ***** allocateAndInitiateQ();
void writeQ(float *****);
int argmax(float * );
int convertIntoZone(int ,int y);
int converterIntoAngleF(float);
int converterIntoAngleH(float);
int takeAction(int ,int , float ***** , int , int, int, float );
void insertPointToLine(ptline_t head, int, int , int , int); 
void freeLine ( ptline_t makeheadLine);
int setReward(int , int , int );
#endif