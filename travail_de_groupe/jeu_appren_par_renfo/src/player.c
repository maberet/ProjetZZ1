#include "player.h"

player_t player;

void initPlayer(){
    player.x= 2*BLOCK_SIZE;
    player.y= 2*BLOCK_SIZE;
    player.speed = 1;
    player.isMoving = 0;
    player.HPMax = 3;
    player.currentHP = player.HPMax;
    player.coins = 0;
    player.angle=0;
}