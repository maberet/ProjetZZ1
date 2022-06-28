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

void hitBall(){
    //printf("map edges: %d %d\n",  BLOCK_SIZE * MAP_WIDTH/2,  BLOCK_SIZE *MAP_HEIGHT/2);
    //printf("ray1: %d %d\n", ray1[0], ray1[1]);
    int angleMin = RD * atan2((MAP_WIDTH/2)*BLOCK_SIZE - player.x, player.y);
    int angleMax = 90 + RD * atan2((MAP_WIDTH/2)*BLOCK_SIZE - player.x, MAP_HEIGHT * BLOCK_SIZE - player.y);
    int currAngle = (int) ((player.angle) * RD +90) %360;
    //printf("player angle: %d\n",(int) ((player.angle) * RD +90) %360 );
    printf("distance to ball: %f\n", sqrt(pow(ball.x - player.x, 2) + pow(ball.y - player.y, 2))/BLOCK_SIZE);
    if (sqrt(pow(player.x - ball.x, 2) + pow(player.y - ball.y, 2))/BLOCK_SIZE < HIT_RANGE){
        if (currAngle < angleMax && currAngle > angleMin){
            printf("hit\n");
            if (player.isHitting){
                ball.x = player.x;
                ball.y = player.y;
            }
           printf("valid hit\n");
        }
        else {
            printf("unvalid hit\n");
        }
    }
    //}
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
    hitBall();
}