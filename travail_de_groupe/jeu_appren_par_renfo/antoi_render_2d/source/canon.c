#include "canon.h"

canon_t canon;

void initCanon(){
    canon.width = 20;
    canon.height = 10;
    canon.length = 20;
    canon.x = 100;
    canon.y = 20;
    canon.zone = -1;
}

int getZone(int terrainX, int terrainY, int terrainW, int terrainH){
    int z = -1;
    //en haut à gauche => 1
    if(canon.x<terrainW/2 && canon.y<terrainH/4){
        z = 1;
    }
    //en haut à droite => 2
    else if(canon.x>=terrainW/2 && canon.y<terrainH/4){
        z = 2;
    }
    //en bas à gauche => 3
    else if(canon.x<terrainW/2 && canon.y>=terrainH/4){
        z = 3;
    }
    //en bas à droite => 4
    else{
        z = 4;
    }
    return z;
}