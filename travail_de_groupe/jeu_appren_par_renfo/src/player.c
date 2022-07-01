#include "player.h"

player_t player;
player_t ennemy;

int landingPointPlayerX;
int landingPointPlayerY = 0;

int lastHitPointPlayerX;
int lastHitPointPlayerY;

int rxWall, ryWall;
float distanceWall;

int landingPointPlayerIsFind = 0;

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
    player.angle = 0;
    player.deltax = 1;
    player.deltay = 0;
    player.viewAngle = 0;
}

int generatelandingPointPlayer(int rxWall, float hitIntensity)
{

    int randomPointX = MAP_WIDTH/2 + hitIntensity * 5;

    landingPointPlayerIsFind = 1;
    landingPointEnnemyIsFind = 0;

    return randomPointX;
}

void hitBall()
{
    if (sqrt(pow(player.x - ball.x, 2) + pow(player.y - ball.y, 2)) / BLOCK_SIZE < HIT_RANGE)
    {

        int rxNet, ryNet;
        float distanceNet;
        if (player.startHitBool)
        {
        castSingleRay(player.angle, &distanceWall, &distanceNet, &rxWall, &ryWall, &rxNet, &ryNet);
            // printf("hit\n");
            if (rxWall > MAP_WIDTH/2)
            {

                //cherche et trouve point de chute, UNE SEULE FOIS!
                if(landingPointPlayerIsFind == 0){

                    landingPointPlayerX = generatelandingPointPlayer(rxWall, player.hitIntensity);

                    lastHitPointPlayerX = ball.x;
                    lastHitPointPlayerY = player.h;

                    ball.angle = player.angle;
                    ball.speed = player.hitIntensity * 5;
                   // ball.z = player.h;
                    ball.isHit = 1;
                    ball.isTravelingTo = AI;

                    player.startHitBool = 0;
                }

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
    player.hitIntensityTimer = timer - player.startHitTimer;
}