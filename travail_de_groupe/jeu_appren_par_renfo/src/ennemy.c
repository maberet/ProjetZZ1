#include "player.h"

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
        angleF = defineAngleF(lastHitPoint[0], lastHitPoint[1], landingPoint[0], landingPoint[1]);
        angleF = converterIntoAngleF(angleF);
        angleH = defineAngleH(lastHitPoint[0], landingPoint[0]);
        angleH = converterIntoAngleH(angleH);
        ennemyZone = convertIntoZone(ennemy.x, ennemy.y);
        canonZone = convertIntoZone(lastHitPoint[0], lastHitPoint[1]);
        action = takeAction(ennemy.x, ennemy.y, Q, canonZone, angleH, angleF, 1);
        while (ennemyHasMoved == 0)
        {
            switch (action)
            {
            case BACK:
                if (ennemy.x + BLOCK_SIZE < (MAP_WIDTH-1) * BLOCK_SIZE)
                {
                    ennemy.x += BLOCK_SIZE;
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
                    ennemy.x -= BLOCK_SIZE;
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
                    ennemy.y -= BLOCK_SIZE;
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
                    ennemy.y += BLOCK_SIZE;
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

void manageEnnemy()
{
    manageEnnemyMovement();
}