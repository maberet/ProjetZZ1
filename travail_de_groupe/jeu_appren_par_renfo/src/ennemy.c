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

int * generateLandingPointEnnemy(){
    int *landingPoint = malloc(sizeof(int) * 2);

    int randomPointX = rand() % ((MAP_WIDTH-1)/2);

    landingPoint[0] = randomPointX ;
    landingPoint[1] = 0;
    landingPointIsFind = 1;

    return landingPoint;
}

void ennemyHitBall(){
    if (sqrt(pow(ennemy.x - ball.x, 2) + pow(ennemy.y - ball.y, 2)) / BLOCK_SIZE < HIT_RANGE)
    {
        if (ball.isTravelingTo == AI)
        {
            
            if(landingPointIsFind == 0){
                freeIntList(landingPoint);
                landingPoint = generateLandingPointEnnemy();
            }
            ball.isTravelingTo = PLAYER;
            ball.angle = ennemy.angle;
            ball.speed = HIT_FORCE;
            ball.isHit = 1;
            lastHitPoint[0] = ball.x;
            lastHitPoint[1] = player.h;

            printf("ennemy new lastHitPoint : %d %d\n", lastHitPoint[0]/BLOCK_SIZE, lastHitPoint[1]/BLOCK_SIZE);

            printf("ennemy new landingPoint : %d %d\n", landingPoint[0]/BLOCK_SIZE, landingPoint[1]/BLOCK_SIZE);
        }
    }
}

void manageEnnemy(){
    if (SDL_GetTicks() % 1000 < 100)
    {
        manageEnnemyMovement();
    }
    ennemyHitBall();
}