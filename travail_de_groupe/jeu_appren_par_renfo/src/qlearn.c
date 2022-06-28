#include "qlearn.h"

void moveAgent(agent_t * agent, int choice){
    switch (choice)
    {
    case BACK:
        agent->x += 1*agent->speed; //Avancer 
        break;

    case FOWARD:
        agent->x -= 1*agent->speed; // reculer 
        break;

    case UP:
        agent->y += 1*agent->speed;
        break;

    case DOWN:
        agent->y -= 1*agent->speed;
        break;
    case WAIT: 
        break; 
    }
}

float ***** allocateAndInitiateQ(){
    int i,j,k,l,m;
    
    float ***** q = malloc(sizeof(float ****) * NUMBER_ZONE_RECEIVER); /// alloc player zone 
    if (q==NULL)
    {
        printf("problème d'allocation \n");
        exit(1);
    }

    for(i = 0; i <  NUMBER_ZONE_RECEIVER; i++){  
        q[i] = malloc(sizeof(float ***) * NUMBER_ZONE_SHOOTER ); // alloc shooter zone 
        if (q[i]==NULL)
        { 
            printf("problème d'allocation \n");            
            exit(1);
        }

        for(j = 0; j< NUMBER_ZONE_SHOOTER; j++){
            q[i][j] = malloc(sizeof(float **) * 3 ); // alloc angle hauteur 

            if (q[i][j]==NULL)
            {
                printf("problème d'allocation \n");            
                exit(1);
            }

            for(k = 0; k <3 ; k++){
                q[i][j][k] = malloc(sizeof(float *) * 5 ); // alloc angle plat 

                if (q[i][j][k]==NULL)
                {
                    printf("problème d'allocation \n");            
                    exit(1);
                }
                for(l = 0; l<5 ; l++){
                    q[i][j][k][l] = malloc(sizeof(float ) * 5); //alloc action 

                    if (q[i][j][k][l]==NULL)
                    {
                        printf("problème d'allocation \n");            
                        exit(1);
                    }
                    for (m=0;m <5;m++){
                        q[i][j][k][l][m]=0;
                    }
                        
                }
            }
        }
    }
    return q;
}

// attention il manque 3 lignes
void writeQ(float *****Q){
    int i, j, k, l, m ;
    FILE * fp = fopen("q.txt", "w+");
    for(i = 0; i < NUMBER_ZONE_RECEIVER; i++){
        for(j = 0; j < NUMBER_ZONE_SHOOTER; j++){
            for(k = 0; k < 3; k++){
                for(l= 0; l < 5; l++){
                    for(m= 0; m <5; m++){
                         fprintf(fp, "%f ", Q[i][j][k][l][m]);
                    }fprintf(fp, "\n");
                }
            }
        }
    }fprintf(fp, "\n");
    fflush(fp);
    fclose(fp);
}


int argmax(float * arr){
    int i;
    float max = arr[0];
    int maxIndex = 0;
    printf("argmax:  %f ", arr[0]);
    for(i = 1; i < NUMBER_ACTION; i++){
        printf("%f ", arr[i]);
        if (arr[i] > max){
            max = arr[i];
            maxIndex = i;
        }
    }
    printf("\n");
    return maxIndex;
}

int convertIntoZone(int xAgent,int yAgent){
    int zone; 
    if(xAgent<23 && yAgent<=8){zone=0;} 
    else if(xAgent<31 && yAgent<=8){zone=1;} 
    else if(xAgent<23 && yAgent<=18){zone=2;} 
    else if(xAgent< 31&& yAgent<18){zone=3;}
     
    return zone ; 
}

int converterIntoAngleF(float angleF){
    int angleZone=0;
    long angleFd=(long)(angleF*1000000); 
    printf ("%ld \n ", angleFd);
    if( ((long)(-M_PI/2*1000000)<=angleFd)&&(angleFd<=(long)((-M_PI/2+M_PI/5)*1000000))){angleZone=4;} 
    else if( ((long)((-(M_PI/2)+(M_PI/5))*1000000)<angleFd)&&(angleFd<=(long)((-M_PI/2+2*M_PI/5)*1000000))){angleZone=3;} 
    else if( (angleFd>(long)((-M_PI/2+2*M_PI/5)*1000000))&&(angleFd<=(long)((-M_PI/2+3*M_PI/5)*1000000))){angleZone=2;}
    else if( (angleFd>(long)((-M_PI/2+3*M_PI/5)*1000000))&&(angleFd<=(long)((-M_PI/2+4*M_PI/5)*1000000))){angleZone=1;}
    else if( (angleFd>(long)((-M_PI/2+4*M_PI/5)*1000000))&&(angleFd<=(long)(M_PI/2*1000000))){angleZone=0;}   
    
    return(angleZone);    
} 

int converterIntoAngleH(float angleH){
    int angleZone=0;
    long angleHd=(long)(angleH*1000000); 
    if( (0<=angleHd)&&(angleHd<=(long)((M_PI/6)*1000000))){angleZone=0;} 
    else if(( ((long)((M_PI/6))*1000000)<angleHd)&&(angleHd<=(long)((M_PI/3)*1000000))){angleZone=1;} 
    else if( (angleHd>(long)((M_PI/3)*1000000))&&(angleHd<=(long)((M_PI/2)*1000000))){angleZone=2;} 
    
    return(angleZone);    
} 

int takeAction(int xagent, int yagent, float ***** Q, int canonZone, int angleHZone, int angleFZone, float eps){
    int action;
    int proba = rand() % 100;
    int receiverZone=0;
    if (proba < eps * 100){
        if (xagent > (MAP_WIDTH-1)/2+1 && xagent < MAP_WIDTH- 2 && yagent > 1 && yagent < MAP_HEIGHT - 2){
            action = rand() % 5;// OK cas au centre
        }
        else if (xagent == (MAP_WIDTH-1)/2+1 && yagent > 1 && yagent < MAP_HEIGHT - 2){
            int possibleActions[4] = {1, 2, 3,4};
            action = possibleActions[rand() % 4];// OK cas filet 
        }
        else if (xagent == (MAP_WIDTH-1)/2+1 && yagent == 1){
            int possibleActions[3] = {1, 3, 4};
            action = possibleActions[rand() % 3];// cas en haut a gauche 
        }
        else if (xagent == (MAP_WIDTH-1)/2+1  && yagent==MAP_HEIGHT - 2){
            int possibleActions[3] = {1, 2, 4};
            action = possibleActions[rand() % 3];// cas en bas a gauche 
        }
        else if (yagent ==1 && xagent > (MAP_WIDTH-1)/2+1 && xagent < MAP_WIDTH- 2){
            int possibleActions[4] = {0, 1,3,4};
            action = possibleActions[rand() % 4];// cas en haut au milieu  
        }
        else if (xagent == MAP_WIDTH- 2 && yagent == 1){
            int possibleActions[3] = {0, 3,4};
            action = possibleActions[rand() % 3];// cas en haut a droite
        }
        else if (xagent ==  MAP_WIDTH-2  && yagent <MAP_HEIGHT-2 && yagent>1){
            int possibleActions[4] = {0,2,3,4};
            action = possibleActions[rand() % 4];// cas a droite au milieu 
        }
        else if (xagent == MAP_WIDTH-2 && yagent == MAP_HEIGHT-2){
            int possibleActions[3] = {0, 2,4};
            action = possibleActions[rand() % 3];// cas en bas a droite 
        }
        else if (xagent > (MAP_WIDTH-1)/2+1 && xagent < MAP_WIDTH- 2 && yagent == MAP_HEIGHT-2){
            int possibleActions[4] = {0,1,2,4};
            action = possibleActions[rand() % 4];
        }
        else{
            action = rand() % 5;
        }
    }
    else{
        receiverZone= convertIntoZone(xagent,yagent);
        action = argmax(Q[receiverZone][canonZone][angleHZone][angleFZone]);
        //printf("wtf");
    }
    return action;
}