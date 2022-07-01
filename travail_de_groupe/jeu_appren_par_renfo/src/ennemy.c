#include "player.h"

int landingPointEnnemyX;
int landingPointEnnemyY;

int lastHitPointEnnemyX;
int lastHitPointEnnemyY = 0;

int landingPointEnnemyIsFind = 0;

int angleF;
int angleH;
int ennemyZone;
int canonZone;
int action;

int ennemyHasMoved = 0;

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
    if (ball.isTravelingTo == AI)
    {
        angleF = defineAngleF(lastHitPointPlayerX, lastHitPointPlayerY, landingPointPlayerX, landingPointPlayerY);
        angleF = converterIntoAngleF(angleF);
        angleH = defineAngleH(lastHitPointPlayerX, landingPointPlayerX);
        angleH = converterIntoAngleH(angleH);
        ennemyZone = convertIntoZone(ennemy.x, ennemy.y);
        canonZone = convertIntoZone(lastHitPointPlayerX, lastHitPointPlayerY);
        action = takeAction(ennemy.x, ennemy.y, Q, canonZone, angleH, angleF, 1);
        while (ennemyHasMoved == 0)
        {
            switch (action)
            {
            case BACK:
                if (ennemy.x + BLOCK_SIZE < (MAP_WIDTH-1) * BLOCK_SIZE)
                {
                    ennemy.x += BLOCK_SIZE/2;
                    ennemyHasMoved = 1;
                }
                else
                {
                    action = takeAction(ennemy.x, ennemy.y, Q, canonZone, angleH, angleF, 1);
                }
                break;

            case FOWARD:
                if (ennemy.x - BLOCK_SIZE > (MAP_WIDTH/2 + 1) * BLOCK_SIZE)
                {
                    ennemy.x -= BLOCK_SIZE/2;
                    ennemyHasMoved = 1;
                }
                else
                {
                    action = takeAction(ennemy.x, ennemy.y, Q, canonZone, angleH, angleF, 1);
                }
                break;

            case UP:
                if (ennemy.y - BLOCK_SIZE > 1)
                {
                    ennemy.y -= BLOCK_SIZE/2;
                    ennemyHasMoved = 1;
                }
                else
                {
                    action = takeAction(ennemy.x, ennemy.y, Q, canonZone, angleH, angleF, 1);
                }
                break;

            case DOWN:
                if (ennemy.y + BLOCK_SIZE < (MAP_HEIGHT-1) * BLOCK_SIZE)
                {
                    ennemy.y += BLOCK_SIZE/2;
                    ennemyHasMoved = 1;
                }
                else
                {
                    action = takeAction(ennemy.x, ennemy.y, Q, canonZone, angleH, angleF, 1);
                }
                break;

            default:
                ennemyHasMoved = 1;
                break;
            }
        }
        ennemyHasMoved = 0;
    }
}

int generatelandingPointEnnemy(){

    int randomPointX = rand() % ((MAP_WIDTH-1)/2);

    landingPointEnnemyIsFind = 1;
    landingPointPlayerIsFind = 0;

    return randomPointX;
}


void ennemyHitBall(){
    if (sqrt(pow(ennemy.x - ball.x, 2) + pow(ennemy.y - ball.y, 2)) / BLOCK_SIZE < HIT_RANGE)
    {
        if (ball.isTravelingTo == AI)
        {
            
                //cherche et trouve point de chute, UNE SEULE FOIS!
            if(landingPointEnnemyIsFind == 0){
                landingPointEnnemyX = generatelandingPointEnnemy();
                ball.isTravelingTo = PLAYER;
                ball.angle = ennemy.angle;
                ball.speed = 4 * HIT_FORCE;
                ball.isHit = 1;
                ball.z = ball.z;
                lastHitPointEnnemyX = ball.x;
                lastHitPointEnnemyY = player.h;
                }

        }
    }
}

void manageEnnemy(){

    manageEnnemyMovement();

    ennemyHitBall();
}