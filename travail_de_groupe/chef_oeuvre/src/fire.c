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
    if (!searchFire(listFire,fire.x,fire.y)){
    m->fire=fire;
    m->next= listFire;
    listFire=m;}

    return listFire;

}

listchainfire_t startFire(listchainfire_t listFire,int numberFire, int mapSize){

    int i; 
    int xFire=0;
    int yFire=0;

    fire_t fire;

    if (emptyListFire(listFire)){
        for (i=0;i<numberFire;i++){
            srand(time(NULL));
            xFire= rand()%mapSize;
            srand(time(NULL));
            yFire= rand()%mapSize;

            fire.x= xFire;
            fire.y= yFire; 
            fire.state=1;

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
        if(((listFire->fire).x==x)&&((listFire->fire).y==y)) // si on trouve la semaine voulue
        {
            result= true ;// on chercher si on trouve l'action ou non.
        }
        listFire=listFire->next; // on passe au suivant.
    }
    return result;
}

listchainfire_t deleteAheadFire(listchainfire_t listFire){

    listchainfire_t listTemporary; 
    
    listTemporary = listFire; // recuperation de la semaine en tête de liste
    listFire = listFire->next; // on avance la liste sur le maillon suivant
    free(listTemporary); // on libere le maillon en tete
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
    return listFire;
}

listchainfire_t offFire (listchainfire_t listFire, int x ,int y ){

    if (emptyListFire(listFire)){
        return(listFire);
    }
    if(((listFire->fire).x==x)&&((listFire->fire).y==y)) {
        (listFire->fire).state=0; // feu eteint
    }
    else{
        listFire->next=offFire(listFire->next,x,y);
    }
    return listFire; 
}
