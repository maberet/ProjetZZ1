#include "player.h"

player_t player;

void initPlayer(){
    player.x= 4*BLOCK_SIZE;
    player.y= 4*BLOCK_SIZE;
    player.speed = 1;
    player.isMoving = 0;
    player.HPMax = 3;
    player.currentHP = player.HPMax;
    player.coins = 0;
    player.angle=0;
    player.deltax = cos(player.angle);
    player.deltay = -sin(player.angle);
    player.viewAngle = 0;
}

void manageMovement(){
    float x_increment = (Keys[0] - Keys[2]) * player.deltax + (Keys[3] - Keys[1]) * sin(player.angle);
    float y_increment = (Keys[0] - Keys[2]) * player.deltay + (Keys[1] - Keys[3]) * cos(player.angle);
    float newpos_x = (player.x + x_increment)/BLOCK_SIZE;
    float newpos_y = (player.y + y_increment)/BLOCK_SIZE;
    if (newpos_x >= 0 && newpos_x < MAP_WIDTH && newpos_y >= 0 && newpos_y < MAP_HEIGHT){
        if (map[(int)newpos_y][(int)newpos_x] == 0){
            player.x += x_increment * MOVEMENT_SPEED;
            player.y += y_increment * MOVEMENT_SPEED;
        }
    }
}

void managePlayer(){
    manageMovement();
}