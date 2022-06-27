#include "canon.h"

canon_t canon;

void initCanon(){
    canon.width = 20;
    canon.height = 10;
    canon.length = 20;
    canon.x = 100;
    canon.y = 20;
    canon.zone = 1;
}

int getZone(int terrainX, int terrainY, int terrainW, int terrainH){
    int z = -1;
    int xCanonTopView = terrainX + canon.x;
    int yCanonTopView = terrainY + canon.y;
    //en haut à gauche => 1
    if(canon.x >= 0 && canon.x<terrainW/2 && yCanonTopView>=0 && canon.y<terrainH/4){
        z = 1;
    }
    //en haut à droite => 2
    else if(xCanonTopView>=terrainX+terrainW/2 && xCanonTopView<=terrainX+terrainW && yCanonTopView>=terrainY && yCanonTopView<terrainX+terrainH/4){
        z = 2;
    }
    //en bas à gauche => 3
    else if(xCanonTopView >= terrainX && xCanonTopView<terrainX+terrainW/2 && yCanonTopView>=terrainY+terrainH/4 && yCanonTopView<=terrainH+terrainH/2){
        z = 3;
    }
    //en bas à droite => 4
    else{
        z = 4;
    }
    return z;
}