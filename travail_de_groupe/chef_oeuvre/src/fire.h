#ifndef PLAYER_H
#define PLAYER_H

#include "main.h"
#include "gest_event.h"

typedef enum                                                
{ 
    false,true
}booleen_t;

typedef struct fire{
    int x;
    int y; 
    int state; // { dead 0 ;sarkle 1 ; medium_fire 2 ; strong_fire 3}

}fire_t;

typedef struct chain {
    fire_t fire; 

    struct chain *next;
}chainfire_t,*listchainfire_t;

listchainfire_t initFire(void);
listchainfire_t insertAheadFire(fire_t, listchainfire_t); 
booleen_t emptyListFire(listchainfire_t);



#endif