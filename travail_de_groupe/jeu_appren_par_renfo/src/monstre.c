#include "monstre.h"
#include "player.h"

monster_t monster; 
int markov[SIZEMARKOV][SIZEMARKOV]; 

void  initMonster (){ 

    srand(time(NULL)); 

    monster.x= (rand()%((MAPSIZEX-3)/2)+16)*CELLSIZE;
    monster.y= (rand()%(MAPSIZEY-2))*CELLSIZE;
    monster.state= PEACEFULL;
    monster.healthPoint= 3; 
    monster.speed=1; 
    monter.angle=0; 
    //monster.dammage

}

void readMarkovFromFile(char * filename){
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


void moveMonsterPeacefull(int markov){
    srand(time(NULL)); 

    probability= rand()%100;

    state=monster.stateMarkov;

    pDead=markov[state][ENTITIES_UP];
    pSparkle=(markov[state][ENTITIES_UP]+markov[state][ENTITIES_DOWN]);

    pMedium=(markov[state][ENTITIES_UP]+markov[state][ENTITIES_DOWN]+markov[state][ENTITIES_LEFT]);

    pStrong=pMedium+markov[state][ENTITIES_RIGHT]);


    if ((0<=probability)&&(probability<pDead)){(listTemporary->fire).state=DEAD;}
    else if ((pDead<=probability)&&(probability<pSparkle)){(listTemporary->fire).state=SPARKLE;}
    else if ((pSparkle<=probability)&&(probability<pMedium)){(listTemporary->fire).state=MEDIUM;}
    else if ((pMedium<=probability)&&(probability<pStrong)){(listTemporary->fire).state=STRONG;}
    else {(listTemporary->fire).state=SPREAD;}


    
    
}

void moveMonsterAngry()