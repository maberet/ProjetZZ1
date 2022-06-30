#include "player.h"

int angleF;
int angleH;
int ennemyZone;
int canonZone;
int action;

void initEnnemy()
{
    ennemy.h = 2 * BLOCK_SIZE;
    ennemy.w = 2 * BLOCK_SIZE;
    ennemy.x = 25 * BLOCK_SIZE;
    ennemy.y = 5 * BLOCK_SIZE;
    ennemy.angle = -pi;
    ennemy.speed = MOVEMENT_SPEED;
}

void manageEnnemyMovement()
{
    if (ball.isTravelingTo == AI){
        //angleF = defineAngleF(lastHitPoint[0], lastHitPoint[1], landingPoint[0], landingPoint[1]);
        //angleF = converterIntoAngleF(angleF);
        //angleH = defineAngleH(lastHitPoint[0], landingPoint[0]);
        //angleH = converterIntoAngleH(angleH);
        //ennemyZone = convertIntoZone(ennemy.x, ennemy.y);
        //canonZone = convertIntoZone(lastHitPoint[0], lastHitPoint[1]);
        action = 0;//takeAction(ennemy.x, ennemy.y, Q, canonZone, angleH, angleF, 0);
        switch (action)
        {
            case BACK:
                ennemy.x += MOVEMENT_SPEED;
                break;

            case FOWARD:
                ennemy.x -= MOVEMENT_SPEED;
                break;

            case UP:
                ennemy.y -= MOVEMENT_SPEED;
                break;

            case DOWN:
                ennemy.y += MOVEMENT_SPEED;
                break;
            
            default:
                break;
        }
    }
}

void manageEnnemy(){
    manageEnnemyMovement();
}