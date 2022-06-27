#include "player.h"

player_t player;

void initPlayer(){
    player.x= 8*CELLSIZE;
    player.y= 8*CELLSIZE;
    player.speed = 1;
    player.isMoving = 0;
    player.HPMax = 3;
    player.currentHP = player.HPMax;
    player.coins = 0;
    player.angle=0;
}