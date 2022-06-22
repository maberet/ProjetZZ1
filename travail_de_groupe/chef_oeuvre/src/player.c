#include "player.h"

player_t player;

void initPlayer(){
    player.x = 0;
    player.y = 0;
    player.waterLevel = 0;
    player.speed = 1;
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
    int distance = sqrt(pow(abs(mousePosition.x - player.x/CELLSIZE), 2)+pow(abs(mousePosition.y - player.y/CELLSIZE), 2));
    if(distance <= 1){
        stateHover = 1;
    }
    return stateHover;
}




