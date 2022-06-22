#include "player.h"

player_t player;

void initPlayer(){
    player.x = 0;
    player.y = 0;
    player.waterLevel = 0;
}

void manageMovement(){
    if(keys[0] == 1){
        player.y--;
    }
    if(keys[1] == 1){
        player.y++;
    }
    if(keys[2] == 1){
        player.x--;
    }
    if(keys[3] == 1){
        player.x++;
    }
}
