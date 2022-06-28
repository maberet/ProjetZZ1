#include "player.h"

player_t player;

player_t ennemy;

player_t ball;

void initPlayer(){
    player.x= 6*BLOCK_SIZE;
    player.y= 6*BLOCK_SIZE;
    player.h = 2 * BLOCK_SIZE;
    player.w = 2 * BLOCK_SIZE;
    ennemy.h = 2 * BLOCK_SIZE;
    ennemy.w = 2 * BLOCK_SIZE;
    ennemy.x = 20 * BLOCK_SIZE;
    ennemy.y = 15 * BLOCK_SIZE;
    ball.h = 0.5 * BLOCK_SIZE;
    ball.w = 0.5 * BLOCK_SIZE;
    ball.x = 10 * BLOCK_SIZE;
    ball.y = 10 * BLOCK_SIZE;
    player.speed = 1;
    player.isMoving = 0;
    player.HPMax = 3;
    player.currentHP = player.HPMax;
    player.coins = 0;
    player.angle=0;
    player.deltax = 0;
    player.deltay = 1;
    player.viewAngle = 0;
}

void manageMovement(){
    float x_increment = (Keys[0] - Keys[2]) * player.deltax + (Keys[3] - Keys[1]) * sin(player.angle);
    float y_increment = (Keys[0] - Keys[2]) * player.deltay + (Keys[1] - Keys[3]) * cos(player.angle);
    float newpos_x = (player.x + x_increment)/BLOCK_SIZE;
    float newpos_y = (player.y + y_increment)/BLOCK_SIZE;
    if (newpos_x >= 0 && newpos_x < MAP_WIDTH && newpos_y >= 0 && newpos_y < MAP_HEIGHT){
        if (map[(int)newpos_y][(int)newpos_x] != 1){
            player.x += x_increment * MOVEMENT_SPEED;
            player.y += y_increment * MOVEMENT_SPEED;
        }
    }
}

void managePlayer(){
    manageMovement();
}