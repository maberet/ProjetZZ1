#include "player.h"

Player_t player;

void initPlayer(){
    player.x = 0;
    player.y = 0;
    player.water_level = 0;
}

void GestMovement(){
    if(Keys[0] == 1){
        player.y--;
    }
    if(Keys[1] == 1){
        player.y++;
    }
    if(Keys[2] == 1){
        player.x--;
    }
    if(Keys[3] == 1){
        player.x++;
    }
}
