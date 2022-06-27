#ifndef MONSTRE_H
#define MONSTRE_H

#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define ANGRY 1 
#define PEACEFULL 0 
#define SIZEMARKOV 4

typedef enum {
 false,true
}booleen_t; 

typedef struct  monster { 
    int x ; 
    int y; 
    int healthPoint; 
    int stateMarkov; 
    int state;
    int speed; 
    float angle; 
}monster_t; 

extern monster_t monster;

void  initMonster ();
void readMarkovFromFile(char * filename);


#endif 