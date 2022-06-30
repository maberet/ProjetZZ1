#ifndef Q_LEARN
#define Q_LEARN

#include <stdio.h>
#include <stdlib.h>
#include <time.h>   
#include "ball.h"
#include "math.h"
#include "map.h"

#define M_PI 3.14159265358979323846

#define LEARN_RATE 0.8
#define DISCOUNT 0.2




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
    int action;
    int reward; 
}line_t; 

typedef struct stack
{
	line_t *base;  
	int 	   numberelt;  
	int 	   top; 

} stack_t;



agent_t* initAgent ( );
void moveAgent(agent_t * agent, int choice);
float ***** allocateAndInitiateQ();
void writeQ(float *****);
int argmax(float * );
int convertIntoZone(int ,int y);
int convertIntoZoneCanon(int xCanon,int yCanon);
int converterIntoAngleF(float);
int converterIntoAngleH(float);
int takeAction(int xAgent, int yAgent, float ***** Q, int canonZone, int angleHZone, int angleFZone, float eps);
int setReward(int , int , int );
stack_t* initStack (int nbelt);
int emptyStack (stack_t *stack);
int fullStack(stack_t *stack);
void actionStack(stack_t *stack, line_t line);
line_t unStack(stack_t *stack);
void freeStack(stack_t *stack);
void traningAgent( int numberRun, int numberStep, float *****Q);
void readQFromFile(float *****Q);
#endif