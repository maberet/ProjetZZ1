#include "player.h"

player_t player;
player_t ennemy;

int *landingPoint;
int *lastHitPoint;

void initPlayer()
{
    player.x = 3 * BLOCK_SIZE;
    player.y = 5 * BLOCK_SIZE;
    player.h = 2 * BLOCK_SIZE;
    player.w = 2 * BLOCK_SIZE;
    player.angle = 0;
    ennemy.h = 2 * BLOCK_SIZE;
    ennemy.w = 2 * BLOCK_SIZE;
    ennemy.x = 25 * BLOCK_SIZE;
    ennemy.y = 5 * BLOCK_SIZE;
    ennemy.angle = -pi;
    player.speed = 1;
    player.isMoving = 0;
    player.HPMax = 3;
    player.currentHP = player.HPMax;
    player.coins = 0;
    player.angle = 0;
    player.deltax = 1;
    player.deltay = 0;
    player.viewAngle = 0;
}

int *generateLandingPoint()
{
    int *landingPoint = malloc(sizeof(int) * 2);
    int randomLength = rand() % (int)((MAP_WIDTH / 2) * BLOCK_SIZE) + (MAP_WIDTH / 2) * BLOCK_SIZE;
    int randomPointX = randomLength * cos(player.angle) + player.x;
    int randomPointY = randomLength * sin(player.angle) + player.y;
    landingPoint[0] = randomPointX / BLOCK_SIZE;
    landingPoint[1] = randomPointY / BLOCK_SIZE;
    // printf("landing point: %d %d\n", landingPoint[0], landingPoint[1]);
    return landingPoint;
}

int *allocLastHitPoint()
{
    int *lastHitPoint = (int *)malloc(sizeof(int) * 2);
    lastHitPoint[0] = 0;
    lastHitPoint[1] = 0;
    return lastHitPoint;
}

void freeIntList(int *list)
{
    if (list != NULL)
    {
        free(list);
    }
}

void hitBall()
{
    // printf("map edges: %d %d\n",  BLOCK_SIZE * MAP_WIDTH/2,  BLOCK_SIZE *MAP_HEIGHT/2);
    // printf("ray1: %d %d\n", ray1[0], ray1[1]);
    int angleMin = RD * atan2((MAP_WIDTH / 2) * BLOCK_SIZE - player.x, player.y);
    int angleMax = 90 + RD * atan2((MAP_WIDTH / 2) * BLOCK_SIZE - player.x, MAP_HEIGHT * BLOCK_SIZE - player.y);
    int currAngle = (int)((player.angle) * RD + 90) % 360;
    // printf("player angle: %d\n",(int) ((player.angle) * RD +90) %360);
    // printf("distance to ball: %f\n", sqrt(pow(ball.x - player.x, 2) + pow(ball.y - player.y, 2))/BLOCK_SIZE);
    if (sqrt(pow(player.x - ball.x, 2) + pow(player.y - ball.y, 2)) / BLOCK_SIZE < HIT_RANGE)
    {
        if (currAngle < angleMax && currAngle > angleMin)
        {
            // printf("hit\n");
            if (player.isHitting)
            {
                freeIntList(landingPoint);
                freeIntList(lastHitPoint);
                int rxWall, ryWall;
                float distanceWall;

                int rxNet, ryNet;
                float distanceNet;
                castSingleRay(player.angle, &distanceWall, &distanceNet, &rxWall, &ryWall, &rxNet, &ryNet);
                printf("rayWall: %d %d\n", rxWall/BLOCK_SIZE, ryWall/BLOCK_SIZE);
                printf("distance: %f\n", distanceWall);

                printf("rayNet: %d %d\n", rxNet/BLOCK_SIZE, ryNet/BLOCK_SIZE);
                printf("distance: %f\n", distanceNet);
                lastHitPoint = allocLastHitPoint();
                landingPoint = generateLandingPoint();
                lastHitPoint[0] = ball.x;
                lastHitPoint[1] = player.h;
                /*ball.x = player.x;
                ball.y = player.y;
                ball.z = player.h;*/
                ball.angle = player.angle;
                ball.speed = 2 * HIT_FORCE;
                ball.z = player.h;
                ball.isHit = 1;
            }
            // printf("valid hit\n");
        }
        else
        {
            // printf("unvalid hit\n");
        }
    }
    //}
}

void updateBall()
{
    ball.x = ball.x + ball.speed * cos(ball.angle);
    ball.y = ball.y + ball.speed * sin(ball.angle);
    if (ball.isHit)
    {
        // landingPoint est déjà / BLOCK_SIZE de base
        ball.z = lagrangeInterpolation(ball.x / BLOCK_SIZE, lastHitPoint[0] / BLOCK_SIZE, lastHitPoint[1] / BLOCK_SIZE, 15, 2 * player.h / BLOCK_SIZE, landingPoint[0], 0);
        if (ball.z > 0)
        {
            /*printf("param. lagrange : xp=%f, xd=%d, yd=%d, xf=%d, yf=%d, xt=%d, yt=%d\n",
                   ball.x / BLOCK_SIZE,
                   lastHitPoint[0] / BLOCK_SIZE,
                   lastHitPoint[1] / BLOCK_SIZE,
                   15,
                   2 * player.h / BLOCK_SIZE,
                   landingPoint[0],
                   0);
            printf("ballZ: %f\n", ball.z);*/
        }
    }

    if ((int)ball.z == 0)
    {
        ball.x = 0;
        ball.y = 0;
        ball.z = 0;
        ball.speed = 0;
    }
}

void manageMovement()
{
    // z : keys[2] : avance
    // s : keys[0] : arrière
    // q : keys[1] : gauche
    // d : keys[3] : droite
    float x_increment = (Keys[0] - Keys[2]) * player.deltax + (Keys[3] - Keys[1]) * sin(player.angle);
    float y_increment = (Keys[0] - Keys[2]) * player.deltay + (Keys[1] - Keys[3]) * cos(player.angle);
    float newpos_x = (player.x + x_increment) / BLOCK_SIZE;
    float newpos_y = (player.y + y_increment) / BLOCK_SIZE;
    if (newpos_x >= 0 && newpos_x < MAP_WIDTH && newpos_y >= 0 && newpos_y < MAP_HEIGHT)
    {
        if (map[(int)newpos_y][(int)newpos_x] != 1)
        {
            player.x += x_increment * MOVEMENT_SPEED;
            player.y += y_increment * MOVEMENT_SPEED;
        }
    }
}

void managePlayer()
{
    manageMovement();
    hitBall();
    updateBall();
}