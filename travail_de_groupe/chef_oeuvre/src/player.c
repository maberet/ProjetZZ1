#include "player.h"

player_t player;

void initPlayer(){
    player.x = 18*32;
    player.y = 0;
    player.w = CELLSIZE;
    player.h = CELLSIZE;
    player.waterLevel = 0;
    player.speed = 1;
}

int collisionsLeftPlayer(){
    int collision = 0;
    if(player.x <= 0){
        collision = 1;
    }
    return collision;
}

int collisionsRightPlayer(){
    int collision = 0;
    if(player.x + player.w >= (MAPSIZE-1) * CELLSIZE){
        collision = 1;
    }
    return collision;
}

int collisionsUpPlayer(){
    int collision = 0;
    if(player.y <= 0){
        collision = 1;
    }
    return collision;
}

int collisionsDownPlayer(){
    int collision = 0;
    if(player.y + player.h >= (MAPSIZE-1) * CELLSIZE){
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
    }
    if(keys[PLAYER_DOWN] == 1){
        moveDownPlayer();
    }
    if(keys[PLAYER_LEFT] == 1){
        moveLeftPlayer();
    }
    if(keys[PLAYER_RIGHT] == 1){
        moveRightPlayer();
    }
}

int selectStateHover(){
    int stateHover = 0;
    int distance = sqrt(pow(abs(hover[0]-player.x), 2)+pow(abs(hover[1]-player.y), 2));
    printf("distance : %d\n", distance);
    if(distance <= 2){
        stateHover = 1;
    }
    return stateHover;
}





