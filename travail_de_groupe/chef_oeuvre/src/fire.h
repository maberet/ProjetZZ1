#ifndef FIRE_H
#define FIRE_H

#include "main.h"
#include "gest_event.h"
#include <time.h>
#include <stdio.h>

#define SIZEMARKOV 4


typedef struct fire{
    int x;
    int y; 
    int state; // { dead 0 ;sarkle 1 ; medium_fire 2 ; strong_fire 3}

}fire_t;

typedef enum                                                
{ 
    false,true
}booleen_t;

typedef struct chain {
    fire_t fire; 

    struct chain *next;
}chainfire_t,*listchainfire_t;

extern listchainfire_t fireList;

listchainfire_t initFire(void);
booleen_t emptyListFire(listchainfire_t); 
listchainfire_t insertAheadFire(fire_t, listchainfire_t); 
listchainfire_t startFire(listchainfire_t ,int , int );
booleen_t searchFire(listchainfire_t, int ,int);
listchainfire_t deleteAheadFire(listchainfire_t);
listchainfire_t deleteFire (listchainfire_t, int ,int );
listchainfire_t freeListFire (listchainfire_t );
listchainfire_t offFire (listchainfire_t, int,int );
void travelFire(listchainfire_t listFire);

#endif