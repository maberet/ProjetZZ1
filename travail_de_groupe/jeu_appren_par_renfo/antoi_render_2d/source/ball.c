#include "ball.h"

ball_t ball;  
laGrange_t coefLagrange; 
int trajectory[NUMBERPOINT][2]; 

void  initBall (){ 


}


double f(int x, float xc, float yc, float xf, float yf, float xt, float yt){
    double returnValue = yf * ((x - xc)/(xf - xc)) * ((x - xt)/(xf - xt)) + yc * ((x - xf)/(xc - xf)) * ((x - xt)/(xc - xt)) + yt * ((x - xc)/(xt - xc)) * ((x - xf)/(xt - xf));
    return returnValue;
}

void laGrange (float beta[3],float Z[3]){ // y , x 
    printf("x canon : %f, x chute : %f, x filet : %f\n", Z[0], Z[1], Z[2]);
    printf("y canon : %f, y chute : %f, y filet : %f\n", beta[0], beta[1], beta[2]);

    beta[0] =-beta[0];
    beta[1] =-beta[1];
    beta[2] =-beta[2];

    coefLagrange.a= Z[0]/((beta[0]-beta[1])*(beta[0]-beta[2]))
                +Z[1]/((beta[1]-beta[0])*(beta[1]-beta[2]))
                +Z[2]/((beta[2]-beta[0])*(beta[2]-beta[1]));

    coefLagrange.b= -(Z[0]*(beta[1]+beta[2]))/((beta[0]-beta[1])*(beta[0]-beta[2]))
                -(Z[1]*(beta[0]+beta[2]))/((beta[1]-beta[0])*(beta[1]-beta[2]))
                -(Z[2]*(beta[0]+beta[1]))/((beta[2]-beta[0])*(beta[2]-beta[1]));

    coefLagrange.c= (Z[0]*beta[1]*beta[2])/((beta[0]-beta[1])*(beta[0]-beta[2]))
                +(Z[1]*beta[0]*beta[2])/((beta[1]-beta[0])*(beta[1]-beta[2]))
                +(Z[2]*beta[0]*beta[1])/((beta[2]-beta[0])*(beta[2]-beta[1]));
}

float defineAngle (canon_t canon, int xDropPoint, int yDropPoint){
    float distance;
    float angleSin;
    distance= sqrtf( powf((float)(xDropPoint-canon.x),2)+powf((float)(yDropPoint-canon.y),2)); 
    angleSin = asinf(distance/(xDropPoint-canon.x));
    return angleSin;
}

void calculationTrajectory(canon_t canon, int xDropPoint, int yDropPoint){
    int i; 
    float setUp[2][3];
    float distance= (float)(yDropPoint-canon.y);


    printf("test : %f\n", y);

    trajectory[0][0]=canon.y;
    trajectory[0][1]=canon.x;

    trajectory[NUMBERPOINT-1][0]=yDropPoint;
    trajectory[NUMBERPOINT-1][1]=xDropPoint;

    setUp[1][0]=(float)canon.y; 
    setUp[0][0]=canon.x; 
    setUp[1][1]=(float)yDropPoint;
    setUp[0][1]=xDropPoint;
    //setUp[0][2]=(13.40/2);// distance du filet dans le repère de frappe
    //filet X
    //terrain.w = 5.20 * zoom;
    // setUp[0][2] = terrain.x + terrain.w + 50 + (13.40 * zoom)/2 -2;
    setUp[0][2] = 50 + 5.20 * 40 + 50 + (13.40 * 40)/2 - 2;
    //setUp[0][2] = 0;

    //filet Y
    // drawerTerrainSideView.y - 1,55 * zoom
    // drawerTerrainSideView.y = terrain.y + terrain.h;
    //terrain.h = 13.40 * zoom 
    //setUp[1][2] = 1.55*40; /// hauteur du filet 
    setUp[1][2] = 50 + 13.40  * 40 - 3.55 * 40;
    //setUp[1][2] = 0;
    
    laGrange(setUp[1],setUp[0]); //laGrange(y, x);

    //printf("coef a:%f; coef b:%f; coef c:%f\n", coefLagrange.a, coefLagrange.b, coefLagrange.c);

    float step= distance / (NUMBERPOINT-1);

    for (i=1; i<NUMBERPOINT-1; i++){
        trajectory[i][0]=(int)((float)canon.y + i*step); 
        trajectory[i][1]=(int)(coefLagrange.c+
                    coefLagrange.b*((float)canon.y + i*step)+
                    coefLagrange.a*(powf((float)canon.y + i*step,2)));
    }

}