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
        angleF = defineAngleF(lastHitPoint[0], lastHitPoint[1], landingPoint[0], landingPoint[1]);
        angleF = converterIntoAngleF(angleF);
        angleH = defineAngleH(lastHitPoint[0], landingPoint[0]);
        angleH = converterIntoAngleH(angleH);
        ennemyZone = convertIntoZone(ennemy.x, ennemy.y);
        canonZone = convertIntoZone(lastHitPoint[0], lastHitPoint[1]);
        action = takeAction(ennemy.x, ennemy.y, Q, canonZone, angleH, angleF, 1);
        switch (action)
        {
            case BACK:
                ennemy.x += BLOCK_SIZE;
                break;

            case FOWARD:
                ennemy.x -= BLOCK_SIZE;
                break;

            case UP:
                ennemy.y -= BLOCK_SIZE;
                break;

            case DOWN:
                ennemy.y += BLOCK_SIZE;
                break;
            
            default:
                break;
        }
    }
}

void generateLandingPointEnnemy(){
    landingPoint[0] = rand() % ((MAP_WIDTH/2));
    landingPoint[1] = rand() % (MAP_HEIGHT);
}

void ennemyHitBall(){
    if (sqrt(pow(ennemy.x - ball.x, 2) + pow(ennemy.y - ball.y, 2)) / BLOCK_SIZE < HIT_RANGE)
    {
        if (ball.isTravelingTo == AI)
        {
            
            ball.isTravelingTo = PLAYER;
            ball.angle = ennemy.angle;
            ball.speed = HIT_FORCE;
            ball.isHit = 1;
            lastHitPoint[0] = ball.x;
            lastHitPoint[1] = player.h;

            printf("new lastHitPoint : %d %d\n", lastHitPoint[0], lastHitPoint[1]);
            generateLandingPointEnnemy();
        }
    }
}

void manageEnnemy(){
    manageEnnemyMovement();
    ennemyHitBall();
}