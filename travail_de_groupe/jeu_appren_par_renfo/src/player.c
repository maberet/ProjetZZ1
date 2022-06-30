#include "player.h"

player_t player;
player_t ennemy;

int *landingPoint;
int *lastHitPoint;
int landingPointIsFind = 0;

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
    player.speed = 100;
    player.isMoving = 0;
    player.HPMax = 3;
    player.currentHP = player.HPMax;
    player.coins = 0;
    player.angle = 0;
    player.deltax = 1;
    player.deltay = 0;
    player.viewAngle = 0;
}

int *generateLandingPoint(int rxWall)
{
    int *landingPoint = malloc(sizeof(int) * 2);
    srand(time(NULL));

    int randomPointX = MAP_WIDTH / 2 + 1 + rand() % (rxWall / BLOCK_SIZE - (MAP_WIDTH / 2));
    int randomPointY = -1;

    landingPoint[0] = randomPointX;
    landingPoint[1] = randomPointY / BLOCK_SIZE;
    landingPointIsFind = 1;

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
    if (sqrt(pow(player.x - ball.x, 2) + pow(player.y - ball.y, 2)) / BLOCK_SIZE < HIT_RANGE)
    {
        int rxWall, ryWall;
        float distanceWall;

        int rxNet, ryNet;
        float distanceNet;
        if (player.isHitting)
        {
            castSingleRay(player.angle, &distanceWall, &distanceNet, &rxWall, &ryWall, &rxNet, &ryNet);
            if (rxWall > MAP_WIDTH / 2)
            {

                freeIntList(lastHitPoint);
                lastHitPoint = allocLastHitPoint();

                // cherche et trouve point de chute, UNE SEULE FOIS!
                if (landingPointIsFind == 0)
                {
                    freeIntList(landingPoint);
                    landingPoint = generateLandingPoint(rxWall);
                    printf("landing point: x=%d; y=%d\n", landingPoint[0], landingPoint[1]);
                }

                lastHitPoint[0] = ball.x;
                lastHitPoint[1] = player.h;

                ball.angle = player.angle;
                ball.speed = 2 * HIT_FORCE;
                ball.z = player.h;
                ball.isHit = 1;
                ball.isTravelingTo = AI;
            }
        }
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
    float newpos_x = (player.x + x_increment * MOVEMENT_SPEED) / BLOCK_SIZE;
    float newpos_y = (player.y + y_increment * MOVEMENT_SPEED) / BLOCK_SIZE;
    int widthColli = player.w / (3 * BLOCK_SIZE);
    if (newpos_x > widthColli && newpos_x < MAP_WIDTH - widthColli && newpos_y > widthColli && newpos_y < MAP_HEIGHT - widthColli)
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
}