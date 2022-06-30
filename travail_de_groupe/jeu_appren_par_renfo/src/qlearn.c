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
                if (agent->x + 1 < (MAP_WIDTH-1) * 1)
                {
                    agent->x += 1;
                }
        
                break;

            case FOWARD:
                if (agent->x - 1 > (MAP_WIDTH/2 + 1) * 1)
                {
                    agent->x -= 1;
                }
            
                break;

            case UP:
                if (agent->y - 1 > 1)
                {
                    agent->y -= 1;
                }
                break;

            case DOWN:
                if (agent->y + 1 < (MAP_HEIGHT-1) * 1)
                {
                    agent->y += 1;
                }
              
                break;
    case WAIT: 
        break; 
    }
}

float ***** allocateAndInitiateQ(){
    int i,j,k,l,m;
    
    float ***** q = malloc(sizeof(float ****) * MAP_WIDTH); /// alloc player zone 
    if (q==NULL)
    {
        printf("problème d'allocation \n");
        exit(1);
    }

    for(i = 0; i <  MAP_WIDTH; i++){  
        q[i] = malloc(sizeof(float ***) * MAP_HEIGHT ); // alloc shooter zone 
        if (q[i]==NULL)
        { 
            printf("problème d'allocation \n");            
            exit(1);
        }

        for(j = 0; j< MAP_HEIGHT; j++){
            q[i][j] = malloc(sizeof(float **) * MAP_WIDTH ); // alloc angle hauteur 

            if (q[i][j]==NULL)
            {
                printf("problème d'allocation \n");            
                exit(1);
            }

            for(k = 0; k <MAP_WIDTH ; k++){
                q[i][j][k] = malloc(sizeof(float *) * MAP_HEIGHT ); // alloc angle plat 

                if (q[i][j][k]==NULL)
                {
                    printf("problème d'allocation \n");            
                    exit(1);
                }
                for(l = 0; l < MAP_HEIGHT; l++){
                    q[i][j][k][l] = malloc(sizeof(float) * NUMBER_ACTION ); // alloc angle bas 
                    if (q[i][j][k][l]==NULL)
                    {
                        printf("problème d'allocation \n");            
                        exit(1);
                    }
                    for(m = 0; m < NUMBER_ACTION; m++){
                        q[i][j][k][l][m] = 0;
                    }
                }

            }
        }
    }
    return q;
}

void writeQToFile(float ***** q, char * filename){
    int i,j,k,l,m;
    FILE * fichier = NULL;
    fichier = fopen(filename, "w");
    if (fichier == NULL)
    {
        printf("erreur d'ouverture du fichier\n");
        exit(1);
    }
    for(i = 0; i <  MAP_WIDTH; i++){  
        for(j = 0; j< MAP_HEIGHT; j++){
            for(k = 0; k <MAP_WIDTH ; k++){
                for(l = 0; l < MAP_HEIGHT; l++){
                    for(m = 0; m < NUMBER_ACTION; m++){
                        fprintf(fichier, "%f ", q[i][j][k][l][m]);
                    }
                }
                fprintf(fichier, "\n");
            }
            fprintf(fichier, "\n");
        }
        fprintf(fichier, "\n");
    }
    fclose(fichier);
}


void readQFromFile(float ***** q, char * filename){
    int i,j,k,l,m;
    FILE * fichier = NULL;
    fichier = fopen(filename, "r");
    if (fichier == NULL)
    {
        printf("erreur d'ouverture du fichier\n");
        exit(1);
    }
    for(i = 0; i <  MAP_WIDTH; i++){  
        for(j = 0; j< MAP_HEIGHT; j++){
            for(k = 0; k <MAP_WIDTH ; k++){
                for(l = 0; l < MAP_HEIGHT; l++){
                    for(m = 0; m < NUMBER_ACTION; m++){
                        fscanf(fichier, "%f ", &q[i][j][k][l][m]);
                    }
                }
            }
        }
    }
    fclose(fichier);
}


int argmax(float * arr){
    int i;
    float max = arr[rand()%5];
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

// int convertIntoZone(int xAgent,int yAgent){
//     int zone=0; 
//     xAgent=xAgent;
//     yAgent=yAgent;
//     if(xAgent> 15 && xAgent<23 && yAgent<=4){zone=0;} 
//     else if(xAgent>22 && xAgent<31 && yAgent<=4){zone=1;} 
//     else if(xAgent> 15 && xAgent<23 && yAgent<9){zone=2;}
//     else if(xAgent> 22 && xAgent<= 31&& yAgent<9){zone=3;}
     
//     return zone ; 
// }

int takeAction(int xAgent, int yAgent, float ***** Q, int landPointX, int landPointY, float eps){
    int action;
    int proba = rand() % 100;
    int receiverZone=0;
    if (proba < eps * 100){
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
        action = argmax(Q[xAgent][yAgent][landPointX][landPointY]);
        //printf("wtf");
    }
    return action;
}


int setReward(int xAgent, int yAgent, int landPointX, int landPointY){ 
    int reward=0; 
    if (xAgent==landPointX && yAgent==landPointY){
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
    int nextAction;
    // point_t canon ; 
    point_t dropPoint ; 
    int reward;
    agent_t *agent; 
    stack_t *stack;
    line_t line; 
    float greedy=1; 
    int maxAction;
    stack= initStack(60000); 
    int xAgent;
    int yAgent;
    int landX;
    int landY;

    int n = 0;
    while (n<numberRun){
        agent=initAgent(); 
        dropPoint=initDropPoint(dropPoint);
        printf("running %d\n", n);
          
        while (agent->x!=dropPoint.x || agent->y!=dropPoint.y){

            reward=0; 
            printf("agent positions : %d %d \n  ",agent->x ,agent->y );
            printf("landing point : %d %d \n", dropPoint.x, dropPoint.y);
            action =takeAction(agent->x ,agent->y , Q, dropPoint.x, dropPoint.y, 0.5);
            printf("action : %d \n", action);
            line.action= action; 
            line.reward= (agent->x==dropPoint.x && agent->y==dropPoint.y); 
            line.landX= dropPoint.x;
            line.landY= dropPoint.y;
            line.xAgent= agent->x;
            line.yAgent= agent->y;
            agent_t nextAgent;
            nextAgent.x = agent->x;
            nextAgent.y = agent->y;
            moveAgent(&nextAgent, action);
            nextAction = argmax(Q[nextAgent.x][nextAgent.y][dropPoint.x][dropPoint.y]);

            Q[agent->x][agent->y][dropPoint.x][dropPoint.y][action] = Q[agent->x][agent->y][dropPoint.x][dropPoint.y][action] + 0.1*(reward + 0.9*Q[nextAgent.x][nextAgent.y][dropPoint.x][dropPoint.y][nextAction] - Q[agent->x][agent->y][dropPoint.x][dropPoint.y][action]);
            //actionStack( stack , line); 
            moveAgent(agent, action);
        }    
                                            
        greedy = greedy - 1/(n+1);
        n++; 
    }
} 