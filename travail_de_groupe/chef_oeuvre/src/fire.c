#include "fire.h"
#include "map.h"

listchainfire_t fireList;
int markov[SIZEMARKOV][SIZEMARKOV];

listchainfire_t initFire (void)
{
    return NULL;
}


booleen_t emptyListFire(listchainfire_t listFire)
{
    booleen_t result = false;

    if (listFire==NULL)
    {
        result=true;
    }
    return (result);
}

listchainfire_t insertAheadFire(fire_t fire,listchainfire_t listFire)
{
    chainfire_t *m;
    m=(chainfire_t*)malloc(sizeof(chainfire_t));
            
    if (m==NULL)
    {
        printf("problème d'allocation \n");
        exit(1);
    }
    if ((!searchFire(listFire,fire.x,fire.y))&&(map[fire.y][fire.x]!=2)){
            m->fire=fire;
            m->next= listFire;
            listFire=m;
    }
    else {
        free(m);
    }
    return listFire;

}

listchainfire_t startFire(listchainfire_t listFire,int numberFire, int mapSize){

    int i; 
    int xFire=0;
    int yFire=0;
    srand(time(NULL));

    fire_t fire;

    if (emptyListFire(listFire)){
        for (i=0;i<numberFire;i++){
            
            xFire= rand()%mapSize;

            yFire= rand()%mapSize;
            fire.x= xFire;
            fire.y= yFire; 
            fire.state=2;

            listFire=insertAheadFire(fire, listFire); 
            
        }
    }
    return (listFire);
}


booleen_t searchFire(listchainfire_t listFire, int x ,int y)
{
    booleen_t       result = false;

    while((listFire!=NULL)&&!(result))
    {  
        if(((listFire->fire).x==x)&&((listFire->fire).y==y)) 
        {
            result= true ;
        }
        listFire=listFire->next; 
    }
    return result;
}

listchainfire_t deleteAheadFire(listchainfire_t listFire){

    listchainfire_t listTemporary; 
    
    listTemporary = listFire; 
    listFire = listFire->next; 
    free(listTemporary); 
    return listFire;
}

listchainfire_t deleteFire (listchainfire_t listFire, int x ,int y ){

    if (emptyListFire(listFire)){
        return(listFire);
    }
    if(((listFire->fire).x==x)&&((listFire->fire).y==y)) {
        listFire= deleteAheadFire( listFire);
    }
    else{
        listFire->next=deleteFire(listFire->next,x,y);
    }
    return listFire; 
}

listchainfire_t freeListFire (listchainfire_t listFire){
    while (emptyListFire(listFire)){
        listFire=deleteAheadFire(listFire);
    }
    listFire=NULL;
    return listFire;
}

listchainfire_t offFire (listchainfire_t listFire, int x ,int y ){

    if (emptyListFire(listFire)){
        return(listFire);
    }
    if(((listFire->fire).x==x)&&((listFire->fire).y==y)) {
        (listFire->fire).state=(listFire->fire).state-1;
    }
    else{
        listFire->next=offFire(listFire->next,x,y);
    }
    return listFire; 
}

void travelFire(listchainfire_t listFire){
    listchainfire_t listTemporary=listFire;

    while(listTemporary!=NULL){
        //printf("x:%d,y:%d\n",(listTemporary->fire).x,(listTemporary->fire).y);
        listTemporary=listTemporary->next;
    }
}

void readFapFromFile(char * filename){
    FILE * fp;
    int i, j;
    fp = fopen(filename, "r");
    if(fp == NULL){
        printf("Error opening file\n");
        exit(1);
    }
    for(i = 0; i < SIZEMARKOV; i++){
        for(j = 0; j < SIZEMARKOV; j++){
            fscanf(fp, "%d", &markov[i][j]);
        }
    }
    fclose(fp);
}

void nextFire(listchainfire_t listFire){
    int probability; 
    int state;
    int pDead;
    int pSparkle;
    int pMedium;
    int pStrong;
    listchainfire_t listTemporary; 

    listTemporary= listFire;
    srand(time(NULL));

    while (!emptyListFire(listTemporary)){
        state=(listTemporary->fire).state;
        probability= rand()%101;

        pDead=markov[state][DEAD];
        pSparkle=(markov[state][SPARKLE]+markov[state][DEAD]);

        pMedium=(markov[state][SPARKLE]+markov[state][DEAD]+markov[state][MEDIUM]);

        pStrong=(markov[state][SPARKLE]+markov[state][DEAD]+
                    markov[state][MEDIUM]+markov[state][STRONG]);


        if ((0<=probability)&&(probability<pDead)){(listTemporary->fire).state=DEAD;}
        else if ((pDead<=probability)&&(probability<pSparkle)){(listTemporary->fire).state=SPARKLE;}
        else if ((pSparkle<=probability)&&(probability<pMedium)){(listTemporary->fire).state=MEDIUM;}
        else if ((pMedium<=probability)&&(probability<pStrong)){(listTemporary->fire).state=STRONG;}
        else {(listTemporary->fire).state=SPREAD;}

        listTemporary=listTemporary->next;   
    }
} 

listchainfire_t probabilitySpreadFire( listchainfire_t listFire, listchainfire_t listTemporary){
    int probability;
    fire_t fire; 
 
    probability= rand()%4;

    if((probability==0)&&((listTemporary->fire).x+1<MAPSIZE)){fire.state=1;
        fire.x=(listTemporary->fire).x+1;
        fire.y=(listTemporary->fire).y;
        listFire=insertAheadFire(fire,listFire);
        }
    if ((probability==1)&&((listTemporary->fire).y+1<MAPSIZE)){fire.state=1;
        fire.x=(listTemporary->fire).x;
        fire.y=(listTemporary->fire).y+1;
        listFire=insertAheadFire(fire,listFire);
        }
    if ((probability==2)&&((listTemporary->fire).y-1>=0)){fire.state=1;
        fire.x=(listTemporary->fire).x;
        fire.y=(listTemporary->fire).y-1;
        listFire=insertAheadFire(fire,listFire);
        }
    if ((probability==3)&&((listTemporary->fire).x-1>=0)){fire.state=1;
        fire.x=(listTemporary->fire).x-1;
        fire.y=(listTemporary->fire).y;
        listFire=insertAheadFire(fire,listFire);
        }
    return listFire;
}

void burnTree (listchainfire_t listFire){
    listchainfire_t listTemporary = listFire; 

    while (!emptyListFire(listTemporary)){
        if (map[(listTemporary->fire).y][(listTemporary->fire).x]==TREE){map[(listTemporary->fire).y][(listTemporary->fire).x]=0;}
    }
}

listchainfire_t spreadFire (listchainfire_t listFire){
    listchainfire_t listTemporary=fireList;
    
    //burnTree(listFire);

    srand(time(NULL));
    while (!emptyListFire(listTemporary)){
        if ((listTemporary->fire).state==4){
            
            listFire=probabilitySpreadFire(listFire, listTemporary); 
        }
        listTemporary=listTemporary->next; 
    }
    return listFire;
}

booleen_t winGame(listchainfire_t listFire){
    booleen_t win = true;

    if(!emptyListFire(listFire)){win=false;}
    
    return win;
}