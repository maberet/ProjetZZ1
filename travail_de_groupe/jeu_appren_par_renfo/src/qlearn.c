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


void writeQ(float *****Q){
    int i, j, k, l, m ;
    FILE * fp = fopen("q.txt", "w+");
    for(i = 0; i < NUMBER_ZONE_RECEIVER; i++){
        for(j = 0; j < NUMBER_ZONE_SHOOTER; j++){
            for(k = 0; k < 3; k++){
                for(l= 0; l < 5; l++){
                    for(m= 0; m <5; m++){
                         fprintf(fp, "%f ", Q[i][j][k][l][m]);
                    }
                }
            }
            fprintf(fp, "\n");
        }
        fprintf(fp, "\n");
    }
    fflush(fp);
    fclose(fp);
}