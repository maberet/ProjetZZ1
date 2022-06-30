#include "ball.h"

ball_t ball;
int whoScored;
int trajectoireAntoine[NUMBERPOINT_TRAJEC][2];

void initBall()
{
    ball.x = player.x + player.w / 2;
    ball.y = player.y;
    ball.z = player.h / BLOCK_SIZE;
    ball.h = 0.5 * BLOCK_SIZE;
    ball.w = 0.5 * BLOCK_SIZE;
    ball.isHit = 0;
    ball.angle = -pi;
    ball.speed = 0;
    landingPointEnnemyIsFind = 0;
    landingPointPlayerIsFind = 0;
}
//ball_t ball;
int trajectoireAntoine[NUMBERPOINT_TRAJEC][2];

point_t initCanon (point_t canon){
    canon.x=(1+rand()%14);
    canon.y=(1+rand()%8);
    return canon; 
} 

point_t initDropPoint (point_t dropPoint){
    dropPoint.x=(16+rand()%14);
    dropPoint.y=(1+rand()%8);
    return dropPoint; 
} 

float defineAngleF(int canonX, int canonY, int xDropPoint, int yDropPoint)
{
    float distance;
    float angleSin;
    canonX=canonX; 
    canonY=canonY; 
    xDropPoint= xDropPoint; 
    yDropPoint= yDropPoint; 

    distance = sqrtf(powf((float)(xDropPoint - canonX), 2) + powf((float)(yDropPoint - canonY), 2));
    angleSin = asinf(((float) (yDropPoint - canonY))/distance);
    return angleSin;
}

float defineAngleH(int xCanon, int xDropPoint){
    int xNet =15 ;
    int yNet= 4; 
    float angle ; 
    float yTemp; 
    xCanon= xCanon;
    xDropPoint=xDropPoint;

    yTemp = lagrangeInterpolation((float)(xCanon+(xDropPoint-xCanon)/4), xCanon , 2,xNet , yNet , xDropPoint, 0 ); 
    angle= atanf( (float)(yTemp/ (xCanon+(xDropPoint-xCanon)/4) )) ;  // possible faute ? 

    return angle; 
} 

/*
 * Fonction qui prend une valeur de x et 3 points. Elle
 * renvoie la coordonnée y liée à la valeur de x sur la
 * courbe passant par les 3 points.
 * Valeur de x : float xp
 * Point n°1 (départ) : int xd, int yd
 * Point n°2 (filet ) : int xf, int yf
 * Point n°3 (target) : int xt, int yt
 */
float lagrangeInterpolation(float xp, int xd, int yd, int xf, int yf, int xt, int yt)
{
    float x[4], y[4], yp = 0, p;
    int i, j, n;
    // nombre de points pour trouver la courbe
    n = 3;
    // coordonnées x des 3 points
    x[1] = xd;
    x[2] = xf;
    x[3] = xt;
    // coordonnées y des 3 points
    y[1] = yd;
    y[2] = yf;
    y[3] = yt;
    // implementation de l'interpolation de Lagrange
    for (i = 1; i <= n; i++)
    {
        p = 1;
        for (j = 1; j <= n; j++)
        {
            if (i != j)
            {
                p = p * (xp - x[j]) / (x[i] - x[j]);
            }
        }
        yp = yp + p * y[i];
    }
    // affichage pouvant servir
    // printf("Interpolated value at %.3f is %.3f.\n", xp, yp);
    // valeur de y associée à xp sur la courbe
    return yp;
}

/*
 * Fonction qui prend en paramètres 3 points et retourne met dans le
 * tableau à 2 dimensions trajectoireAntoine la liste des points de
 * la courbe qui passe par ces 3 points. Le pas de temps est defini
 * par NUMBERPOINT_TRAJEC.
 */
void calculTrajectoireAntoine2(int xd, int yd, int xf, int yf, int xt, int yt)
{
    // pas de temps
    int pas_de_temps = (xt - xd) / (NUMBERPOINT_TRAJEC - 1);
    int i;
    // point de départ
    trajectoireAntoine[0][0] = xd;
    trajectoireAntoine[0][1] = yd;
    // on cherche les pas de temps pour tous les pas de temps
    // sauf départ et arrivée
    for (i = 1; i < NUMBERPOINT_TRAJEC - 1; i++)
    {
        // calcul du x
        trajectoireAntoine[i][0] = (int)(xd + i * pas_de_temps);
        // calcul du y
        trajectoireAntoine[i][1] = lagrangeInterpolation(trajectoireAntoine[i][0], xd, yd, xf, yf, xt, yt);
    }
    // point d'arrivée
    trajectoireAntoine[NUMBERPOINT_TRAJEC - 1][0] = xt;
    trajectoireAntoine[NUMBERPOINT_TRAJEC - 1][1] = yt;
}

void updateBall()
{
    ball.x = ball.x + ball.speed * cos(ball.angle);
    ball.y = ball.y + ball.speed * sin(ball.angle);
    if (ball.isHit)
    {
        // landingPoint est déjà / BLOCK_SIZE de base
        if (ball.isTravelingTo == AI){
            ball.z = lagrangeInterpolation(ball.x / BLOCK_SIZE, lastHitPointPlayerX / BLOCK_SIZE, lastHitPointPlayerY / BLOCK_SIZE, 15, 2 * player.h / BLOCK_SIZE, landingPointPlayerX, 0);
        }
        else if (ball.isTravelingTo == PLAYER){
            ball.z = lagrangeInterpolation(ball.x / BLOCK_SIZE, lastHitPointEnnemyX / BLOCK_SIZE, lastHitPointEnnemyY / BLOCK_SIZE, 15, 2 * ennemy.h / BLOCK_SIZE, landingPointEnnemyX, 0);
        }
    }
    if (ball.z < 0)
    {
        if (ball.x/BLOCK_SIZE < MAP_WIDTH/2){
            whoScored = AI;
        }
        else {
            whoScored = PLAYER;
        }
        ball.z = 0;
        ball.speed = 0;
        //initBall();
    }
    else {
        whoScored = NONE;
    }

}
