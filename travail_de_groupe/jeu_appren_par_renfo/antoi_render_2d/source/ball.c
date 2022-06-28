#include "ball.h"

ball_t ball;
int trajectoireAntoine[NUMBERPOINT_TRAJEC][2];

void initBall()
{
}

float defineAngle(int canonX, int canonY, int xDropPoint, int yDropPoint)
{
    float distance;
    float angleSin;
    distance = sqrtf(powf((float)(xDropPoint - canonX), 2) + powf((float)(yDropPoint - canonY), 2));
    angleSin = asinf(distance / (xDropPoint - canonX));
    return angleSin;
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
int lagrangeInterpolation(float xp, int xd, int yd, int xf, int yf, int xt, int yt)
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
