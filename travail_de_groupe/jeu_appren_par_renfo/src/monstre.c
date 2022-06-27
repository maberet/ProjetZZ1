#include "monster.h"

monster_t monster; 
int markov[SIZEMARKOV][SIZEMARKOV]; 

void  initMonster (){ 

    monster.x= (rand()%((MAPSIZEX-3)/2)+16)*CELLSIZE;
    monster.y= (rand()%(MAPSIZEY-2))*CELLSIZE;
    monster.state= PEACEFULL;
    monster.healthPoint= 3; 
    monster.speed=1; 
    monster.angle=0; 
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
