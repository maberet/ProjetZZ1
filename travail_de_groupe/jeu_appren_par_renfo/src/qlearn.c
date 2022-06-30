#include "qlearn.h"

agent_t * initAgent (){
    agent_t * agent =(agent_t*)malloc(sizeof(agent_t));
    if (agent ==NULL){
        printf("erreur alloc\n  ");
        exit (1);
    }  
    agent->x=(16+rand()%14);
    agent->y=(1+rand()%8);
    agent->high=2;
    agent->weight=2; 
    agent->speed = 1;   
      // si changement de speed => changement de collisiosn dans le takeaction 
      return(agent);
}   

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
                    }fprintf(fp, "\n");
                }
            }
        }
    }fprintf(fp, "\n");
    fflush(fp);
    fclose(fp);
}

void readQFromFile(float *****Q){
    int i, j, k, l, m ;
    FILE * fp = fopen("q.txt", "r");
    for(i = 0; i < NUMBER_ZONE_RECEIVER; i++){
        for(j = 0; j < NUMBER_ZONE_SHOOTER; j++){
            for(k = 0; k < 3; k++){
                for(l= 0; l < 5; l++){
                    for(m= 0; m <5; m++){
                         fscanf(fp, "%f ", &Q[i][j][k][l][m]);
                    }
                }
            }
        }
    }
    fclose(fp);
}


int argmax(float * arr){
    int i;
    float max = arr[0];
    int maxIndex = 0;
    //printf("argmax:  %f ", arr[0]);
    for(i = 1; i < NUMBER_ACTION; i++){
        //printf("%f ", arr[i]);
        if (arr[i] > max){
            max = arr[i];
            maxIndex = i;
        }
    }
    //printf("\n");
    return maxIndex;
}

int convertIntoZone(int xAgent,int yAgent){
    int zone=0; 
    xAgent=xAgent;
    yAgent=yAgent;
    if(xAgent> 15 && xAgent<23 && yAgent<=4){zone=0;} 
    else if(xAgent>22 && xAgent<31 && yAgent<=4){zone=1;} 
    else if(xAgent> 15 && xAgent<23 && yAgent<9){zone=2;}
    else if(xAgent> 22 && xAgent<= 31&& yAgent<9){zone=3;}
     
    return zone ; 
}
int convertIntoZoneCanon(int xCanon,int yCanon){
    int zone=0; 
    xCanon=xCanon;
    yCanon=yCanon;
    if(xCanon<9 && yCanon<=4){zone=0;} 
    else if(xCanon<15 && yCanon<=4){zone=1;} 
    else if(xCanon<9 && yCanon<9){zone=2;} 
    else if(xCanon<15&& yCanon<9){zone=3;}
     
    return zone ; 
}

int converterIntoAngleF(float angleF){
    int angleZone=0;
    long angleFd=(long)(angleF*1000000); 
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

int takeAction(int xAgent, int yAgent, float ***** Q, int canonZone, int angleHZone, int angleFZone, float eps){
    int action;
    int proba = rand() % 10000;
    int receiverZone=0;
    if (proba < eps * 10000){
        if (xAgent > (MAP_WIDTH-1)/2+1 && xAgent < MAP_WIDTH- 2 && yAgent > 1 && yAgent < MAP_HEIGHT - 2){
            action = rand() % 5;// OK cas au centre
        }
        else if (xAgent == (MAP_WIDTH-1)/2+1 && yAgent > 1 && yAgent < MAP_HEIGHT - 2){
            int possibleActions[4] = {1, 2, 3,4};
            action = possibleActions[rand() % 4];// OK cas filet 
        }
        else if (xAgent == (MAP_WIDTH-1)/2+1 && yAgent== 1){
            int possibleActions[3] = {1, 3, 4};
            action = possibleActions[rand() % 3];// cas en haut a gauche 
        }
        else if (xAgent == (MAP_WIDTH-1)/2+1  && yAgent==MAP_HEIGHT - 2){
            int possibleActions[3] = {1, 2, 4};
            action = possibleActions[rand() % 3];// cas en bas a gauche 
        }
        else if (yAgent ==1 && xAgent > (MAP_WIDTH-1)/2+1 && xAgent < MAP_WIDTH- 2){
            int possibleActions[4] = {0, 1,3,4};
            action = possibleActions[rand() % 4];// cas en haut au milieu  
        }
        else if (xAgent == MAP_WIDTH- 2 && yAgent == 1){
            int possibleActions[3] = {0, 3,4};
            action = possibleActions[rand() % 3];// cas en haut a droite
        }
        else if (xAgent ==  MAP_WIDTH-2  && yAgent <MAP_HEIGHT-2 && yAgent>1){
            int possibleActions[4] = {0,2,3,4};
            action = possibleActions[rand() % 4];// cas a droite au milieu 
        }
        else if (xAgent== MAP_WIDTH-2 && yAgent == MAP_HEIGHT-2){
            int possibleActions[3] = {0, 2,4};
            action = possibleActions[rand() % 3];// cas en bas a droite 
        }
        else if (xAgent > (MAP_WIDTH-1)/2+1 && xAgent < MAP_WIDTH- 2 && yAgent == MAP_HEIGHT-2){
            int possibleActions[4] = {0,1,2,4};
            action = possibleActions[rand() % 4];
        }
        else{
            action = rand() % 5;
        }
    }
    else{
        receiverZone= convertIntoZone(xAgent,yAgent);
        action = argmax(Q[receiverZone][canonZone][angleHZone][angleFZone]);
        //printf("wtf");
    }
    return action;
}


int setReward(int xAgent, int yAgent, int dropZone){
    int zoneAgent; 
    int reward=0; 

    zoneAgent= convertIntoZone( xAgent,yAgent); 
    if (zoneAgent==dropZone){
        reward=1;
    } 
    return (reward); 
} 


stack_t* initStack(int numberelt){
    stack_t          *stack=NULL; 

    stack=(stack_t *)malloc(sizeof(stack_t)); // allocation du ptr de tête. 

    if (stack==NULL){  // vérification de l'allocation.   
        printf("problème d'allocation\n");
        exit(1);
    }

    stack->base = (line_t *) malloc(numberelt*sizeof(line_t));// allocation de la stack de longueur numberelt. 

    if (stack->base==NULL){
        printf("problème d'allocation\n");
        exit(1);
    }

    stack->numberelt=numberelt; //ajout du nombre d'élément insérable dans la file.
    stack->top=-1; //initialisation de l'indice du dernier élément.
    
    return (stack);
}


int emptyStack(stack_t *stack){
    
   int        result =0; 

    if (stack->top==-1){   // test de l'indice du top. 
        
        result=1;  // la stack est vide.
    }

    return (result);
}

int fullStack(stack_t *stack)
{
    int result = 0;

    if (stack->numberelt == (stack->top)+1) //si le top est l'indice du dernier élément élément de la stack
    {
        result = 1;
    }

    return result;
}

void actionStack(stack_t *stack, line_t element)
{
    if(!fullStack(stack)) //si la stack n'est pas pleine
    {
        stack->base[(stack->top)+1] = element; //on ajoute l'élément à l'indice top+1
        stack->top = stack->top+1; //on incrémente l'indice du top
    }
    else
    {
        printf("Pile pleine\n" );
    }
}

line_t unStack(stack_t *stack)
{
    line_t top;
    
    if(!emptyStack(stack)) //si la stack n'est pas vide
    {
        top = stack->base[stack->top]; //on récupère le top
        stack->top = stack->top-1; //on décrémente l'indice du top
    }
    else
    {
        printf("Pile vide");
    }

    return top;
}

void freeStack(stack_t *stack)
{
    if(stack != NULL)
    {
        free(stack->base); //on libère le tableau dynamique
        free(stack);    //on libère la tête de la stack
        //printf("Pile libérée\n");
    }
    else
    {
        printf("Libération impossible, stack == NULL\n");
    }
}



void traningAgent ( int numberRun, int numberStep, float *****Q) {// pour avoir la bonne taille il faut diviser par block size 
    int i ; 
    int action;
    point_t canon ; 
    point_t dropPoint ; 
    int canonZone; 
    int dropZone; 
    float angleH;
    float angleF;
    int zoneAngleH;
    int zoneAngleF; 
    int agentZone; 
    int reward;
    agent_t *agent; 
    stack_t *stack;
    line_t line; 
    float greedy=1; 
    int maxAction;
    stack= initStack(6000); 
    
    while (numberRun>0){
        agent=initAgent(); 
        canon=initCanon(canon); 
        dropPoint= initDropPoint(dropPoint);
        angleF=defineAngleF( canon.x, canon.x, dropPoint.x, dropPoint.y );
        angleH=defineAngleH( canon.x, canon.y ); 
        zoneAngleF=converterIntoAngleF(angleF);
        zoneAngleH=converterIntoAngleH(angleH);
        dropZone=convertIntoZone(dropPoint.x,dropPoint.y); 
        canonZone= convertIntoZoneCanon(canon.x,canon.y); 
        reward=0; 
        printf("%d %d %d %d \n",dropZone, canonZone,zoneAngleH,zoneAngleF);
        printf("%d %d  \n",agent->x, agent->y);

        for (i=0; i<numberStep-1;i++){ 
            action = takeAction(agent->x,agent->y,Q,canonZone,zoneAngleH,zoneAngleF,greedy); 
            agentZone = convertIntoZone(agent->x, agent->y); 
            line.receiverZone=agentZone; 
            line.shooterZone =canonZone; 
            line.angleHZone= zoneAngleH; 
            line.angleFZone= zoneAngleF; 
            line.action= action;
            line.reward= reward ; 
            actionStack(stack,line);
            moveAgent(agent, action);

        }
        action = takeAction(agent->x, agent->y,Q,canonZone,zoneAngleH,zoneAngleF,greedy); 
        agentZone = convertIntoZone(agent->x, agent->y); 
       
        line.receiverZone=agentZone; 
        line.shooterZone =canonZone; 
        line.angleHZone= zoneAngleH; 
        line.angleFZone= zoneAngleF; 
        line.action= action;
        line.reward = 0; 
       // actionStack(stack,line);
        moveAgent(agent, action);
         if (agentZone==dropZone){ 
                   reward=1; 
                }
                else{reward= 0;}
        

        Q[line.receiverZone][line.shooterZone][line.angleHZone][line.angleFZone][line.action] +=  
                    + LEARN_RATE* ( reward - Q[line.receiverZone][line.shooterZone][line.angleHZone][line.angleFZone][line.action] );
       
        while (!emptyStack(stack)){
            maxAction= argmax(Q[line.receiverZone][line.shooterZone][line.angleHZone][line.angleFZone]);
            reward=line.reward;
            line=unStack(stack);

            Q[line.receiverZone][line.shooterZone][line.angleHZone][line.angleFZone][line.action] +=  
                    + LEARN_RATE* ( reward +  DISCOUNT*Q[line.receiverZone][line.shooterZone][line.angleHZone][line.angleFZone][maxAction]
                    - Q[line.receiverZone][line.shooterZone][line.angleHZone][line.angleFZone][line.action] );
        }  
        numberRun--; 
        greedy=greedy-1/((float)numberRun);

        if ( numberRun%1000000==1){printf (" %d \n  ", numberRun);} 
    } 
    freeStack(stack);
} 