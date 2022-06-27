#include "ball.h"
#include "player.h"

ball_t ball;  
laGrange_t coefLagrange; 
int trajectory[NUMBERPOINT][2]; 

void  initBall (){ 


}

void laGrange (float beta[3],float Z[3]){

    coefLagrange.a= Z[0]/((beta[0]-beta[1])*(beta[0]-beta[2]))+ Z[1]/((beta[1]-beta[0])*(beta[1]-beta[2]))+Z[2]/((beta[2]-beta[0])*(beta[2]-beta[1]));
    coefLagrange.b= -(Z[0]*(beta[1]+beta[2]))/((beta[0]-beta[1])*(beta[0]-beta[2]))
                -
                (Z[1]*(beta[0]+beta[2]))/((beta[1]-beta[0])*(beta[1]-beta[2]))
                -
                (Z[2]*(beta[0]+beta[1]))/((beta[2]-beta[0])*(beta[2]-beta[1]));

    coefLagrange.c= (Z[0]*(beta[1]*beta[2]))/((beta[0]-beta[1])*(beta[0]-beta[2]))
                +
                (Z[1]*(beta[0]*beta[2]))/((beta[1]-beta[0])*(beta[1]-beta[2]))
                +
                (Z[2]*(beta[0]*beta[1]))/((beta[2]-beta[0])*(beta[2]-beta[1]));
}

float defineAngle (canon_t canon, int xDropPoint, int yDropPoint){
    float distance;
    float angleSin;

    distance= sqrtf( powf((float)(xDropPoint-canon.x),2)+powf((float)(yDropPoint-canon.y),2)); 
    angleSin = asinf(distance/(xDropPoint-canon.x));
    return angleSin;
}

void calculationTrajectory(canon_t canon,float angle, int xDropPoint, int yDropPoint){
    trajectory[0][0]=canon.y;
    trajectory[0][1]=0;
    trajectory[NUMBERPOINT-1][0]=yDropPoint;
    trajectory[NUMBERPOINT-1][1]=0;
    float distance= sqrtf( powf((float)(xDropPoint-canon.x),2)+powf((float)(yDropPoint-canon.y),2));
    float step= distance /(NUMBERPOINT-1)
    for (i=1; i<NUMBERPOINT-1; i++){
        trajectory[i][0]=(int)((float)canon.y + i*step); 
        trajectory[i][1]=(int)(coefLagrange.c+coefLagrange.b*((float)canon.y + i*step)+coefLagrange.a*(powf((float)canon.y + i*step),2))
    }

}









