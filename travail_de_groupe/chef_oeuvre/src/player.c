#include "player.h"

player_t player;


void initPlayer(){
    player.x = 18*32;
    player.y = 0;
    player.w = CELLSIZE*0.8;
    player.h = CELLSIZE*0.8;
    player.waterLevel = 0;
    player.speed = 1;
    player.isMoving = 0;
}

int giveCaseBelowPosition(int x, int y){
    int x2 = x/CELLSIZE;
    int y2 = y/CELLSIZE;
    return map[y2][x2];
}

int collisionsLeftPlayer(){
    int collision = 0;
    int dotTopLeft = giveCaseBelowPosition(player.x, player.y);
    int dotDownLeft = giveCaseBelowPosition(player.x, player.y+player.h);
    //collision window
    if(player.x <= 0){
        collision = 1;
    }
    //collision tree
    if(dotTopLeft==1 || dotDownLeft==1){
        collision = 1;
    }

    return collision;
}

int collisionsRightPlayer(){
    int collision = 0;
    int dotTopRight = giveCaseBelowPosition(player.x+player.w, player.y);
    int dotDownRight = giveCaseBelowPosition(player.x+player.w, player.y+player.h);
    //collision window
    if(player.x + player.w >= MAPSIZE * CELLSIZE){
        collision = 1;
    }
    //collision tree
    if(dotTopRight==1 || dotDownRight==1){
        collision = 1;
    }
    return collision;
}

int collisionsUpPlayer(){
    int collision = 0;
    int dotTopRight = giveCaseBelowPosition(player.x+player.w, player.y);
    int dotTopLeft = giveCaseBelowPosition(player.x, player.y);
    //collision window
    if(player.y <= 0){
        collision = 1;
    }
    //collision tree
    if(dotTopRight==1 || dotTopLeft==1){
        collision = 1;
    }
    return collision;
}

int collisionsDownPlayer(){
    int collision = 0;
    int dotDownRight = giveCaseBelowPosition(player.x+player.w, player.y+player.h);
    int dotDownLeft = giveCaseBelowPosition(player.x, player.y+player.h);
    //collision window
    if(player.y + player.h >= MAPSIZE * CELLSIZE){
        collision = 1;
    }
    //collision tree
    if(dotDownRight==1 || dotDownLeft==1){
        collision = 1;
    }
    return collision;
}

void moveRightPlayer(){
    if(!collisionsRightPlayer()){
        player.x = player.x + player.speed;
    }
}

void moveLeftPlayer(){
    if(!collisionsLeftPlayer()){
        player.x = player.x - player.speed;
    }
}

void moveUpPlayer(){
    if(!collisionsUpPlayer()){
        player.y = player.y - player.speed;
    }
}

void moveDownPlayer(){
    if(!collisionsDownPlayer()){
        player.y = player.y + player.speed;
    }
}

void manageMovement(){
    if(keys[PLAYER_UP]){
        moveUpPlayer();
        player.isMoving = 1;
    }
    if(keys[PLAYER_DOWN]){
        moveDownPlayer();
        player.isMoving = 1;
    }
    if(keys[PLAYER_LEFT]){
        moveLeftPlayer();
        player.isMoving = 1;
    }
    if(keys[PLAYER_RIGHT]){
        moveRightPlayer();
        player.isMoving = 1;
    }
    player.isMoving = 0;
}

int selectStateHover(){
    int stateHover = 0;
    int distance = sqrt(pow(abs(mousePosition.x - ((player.x+player.w/2))/CELLSIZE), 2)+pow(abs(mousePosition.y - ((player.y+player.h/2))/CELLSIZE), 2));
    if(distance <= 1){
        stateHover = 1;
    }
    return stateHover;
}




