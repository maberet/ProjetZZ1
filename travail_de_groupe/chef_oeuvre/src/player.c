#include "player.h"

player_t player;


void initPlayer(){
    player.x = (MAPSIZE/2)*CELLSIZE;
    player.y = (MAPSIZE/2)*CELLSIZE;
    player.w = CELLSIZE*0.8;
    player.h = CELLSIZE*0.8;
    player.waterMax = 3;
    player.currentWater = player.waterMax;
    player.speed = 1;
    player.isMoving = 0;
    player.HPMax = 3;
    player.currentHP = player.HPMax;
    player.invisible = 0;
    player.invisibleTime = 5;
    player.score = 0;
}

int giveCaseBelowPosition(int x, int y){
    int x2 = x/CELLSIZE;
    int y2 = y/CELLSIZE;
    return map[y2][x2];
}

int getPositionXInMap(int x){
    int x2 = x/CELLSIZE;
    return x2;
}

int getPositionYInMap(int y){
    int y2 = y/CELLSIZE;
    return y2;
}

int checkCollisionsTypeGround(int dot1, int dot2, int typeGround){
    int coll = 0;
    if(dot1==typeGround || dot2==typeGround){
        coll = 1;
    }
    return coll;
}

int collisionsCaseSolid(int dot1, int dot2){
    return checkCollisionsTypeGround(dot1, dot2, TREE)
        || checkCollisionsTypeGround(dot1, dot2, WATER);
}

/*int collisionsWindow(){

}*/

void checkCollisionsFire(int dot1, int dot2){
    if(player.invisible==0 && (dot1==1 || dot2==1) && player.currentHP>0){
        player.currentHP = player.currentHP - 1;
        player.invisible = 1;
        player.timeLastHitFire = (int)timer/1000;
    }
}

int collisionsLeftPlayer(int dot1, int dot2){
    int collisionWindow = 0;
    int collisionCaseSolid = 0;
    //collision window
    if(player.x <= 0){
        collisionWindow = 1;
    }
    //collisions
    collisionCaseSolid = collisionsCaseSolid(dot1, dot2);
    return collisionWindow || collisionCaseSolid;
}

int collisionsRightPlayer(int dot1, int dot2){
    int collisionWindow = 0;
    int collisionCaseSolid = 0;
    //collision window
    if(player.x + player.w >= MAPSIZE * CELLSIZE){
        collisionWindow = 1;
    }
    //collisions
    collisionCaseSolid = collisionsCaseSolid(dot1, dot2);
    return collisionWindow || collisionCaseSolid;
}

int collisionsUpPlayer(int dot1, int dot2){
    int collisionWindow = 0;
    int collisionCaseSolid = 0;
    //collision window
    if(player.y <= 0){
        collisionWindow = 1;
    }
    //collisions solide like tree or water or rock(in the future)
    collisionCaseSolid = collisionsCaseSolid(dot1, dot2);
    return collisionWindow || collisionCaseSolid;
}

int collisionsDownPlayer(int dot1, int dot2){
    int collisionWindow = 0;
    int collisionCaseSolid = 0;
    //collision window
    if(player.y + player.h >= MAPSIZE * CELLSIZE){
        collisionWindow = 1;
    }
    //collisions
    collisionCaseSolid = collisionsCaseSolid(dot1, dot2);
    return collisionWindow || collisionCaseSolid;
}

void moveRightPlayer(){
    player.x = player.x + player.speed;
}

void moveLeftPlayer(){
    player.x = player.x - player.speed;
}

void moveUpPlayer(){
    player.y = player.y - player.speed;
}

void moveDownPlayer(){
    player.y = player.y + player.speed;
}

void manageMovement(){
    int dotTopRight = giveCaseBelowPosition(player.x+player.w, player.y);
    int dotTopLeft = giveCaseBelowPosition(player.x, player.y);
    int dotDownRight = giveCaseBelowPosition(player.x+player.w, player.y+player.h);
    int dotDownLeft = giveCaseBelowPosition(player.x, player.y+player.h);
    int fireTopRight = searchFire(fireList, getPositionXInMap(player.x+player.w), getPositionYInMap(player.y));
    int fireTopLeft = searchFire(fireList, getPositionXInMap(player.x), getPositionYInMap(player.y));
    int fireDownRight = searchFire(fireList, getPositionXInMap(player.x+player.w), getPositionYInMap(player.y+player.h));
    int fireDownLeft = searchFire(fireList, getPositionXInMap(player.x), getPositionYInMap(player.y+player.h));

    if(keys[PLAYER_UP]){
        if(!collisionsUpPlayer(dotTopRight, dotTopLeft)){
            moveUpPlayer();
            checkCollisionsFire(fireTopRight, fireTopLeft);
            player.isMoving = 1;
            player.direction = PLAYER_UP;
        }
    }
    else if(keys[PLAYER_DOWN]){
        if(!collisionsDownPlayer(dotDownRight, dotDownLeft)){
            moveDownPlayer();
            checkCollisionsFire(fireDownRight, fireDownLeft);
            player.isMoving = 1;
            player.direction = PLAYER_DOWN;
        }
    }
    else if(keys[PLAYER_LEFT]){
        if(!collisionsLeftPlayer(dotTopLeft, dotDownLeft)){
            moveLeftPlayer();
            checkCollisionsFire(fireTopLeft, fireDownLeft);
            player.isMoving = 1;
            player.direction = PLAYER_LEFT;
        }
    }
    else if(keys[PLAYER_RIGHT]){
        if(!collisionsRightPlayer(dotTopRight, dotDownRight)){
            moveRightPlayer();
            checkCollisionsFire(fireTopRight, fireDownRight);
            player.isMoving = 1;
            player.direction = PLAYER_RIGHT;    
        }
    }
    else player.isMoving = 0;
    //
    if(player.invisible==1){
        if((int)timer/1000 - player.timeLastHitFire > player.invisibleTime){
            player.invisible = 0;
        }
    }
}

int selectStateHover(){
    int stateHover = 0;
    int distance = sqrt(pow(abs(mousePosition.x - ((player.x+player.w/2))/CELLSIZE), 2)+pow(abs(mousePosition.y - ((player.y+player.h/2))/CELLSIZE), 2));
    if(distance <= 1){
        stateHover = 1;
    }
    return stateHover;
}

void pourWater(int x, int y){
    int onFire = searchFire(fireList, x, y);     
    if(player.currentWater>0){
        //downgrade the cliqued fire
        if(onFire == 1 && selectStateHover() == 1){ 
            fireList = deleteFire (fireList, x, y); 
            player.score+=1;
        }
        player.currentWater = player.currentWater - 1;
    }
}

void fillWater(int x, int y){    
    if(player.currentWater<player.waterMax && selectStateHover() == 1){
        player.currentWater = player.currentWater + 1;
    }
}

void updatePlayer(){
    if (player.currentHP <= 0){
        gameState = LOSE;
    }
    manageMovement();
}

void initAll(){
    fireList = freeListFire(fireList);
    fireList = startFire(fireList,10,MAPSIZE);
    initPlayer();
}


