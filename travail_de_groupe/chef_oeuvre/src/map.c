#include "map.h"

int map[MAPSIZE][MAPSIZE];

void readMapFromFile(char * filename){
    FILE * fp;
    int i, j;
    fp = fopen(filename, "r");
    if(fp == NULL){
        printf("Error opening file\n");
        exit(1);
    }
    for(i = 0; i < MAPSIZE; i++){
        for(j = 0; j < MAPSIZE; j++){
            fscanf(fp, "%d", &map[i][j]);
        }
    }
    fclose(fp);
}

void updateMap(){
    listchainfire_t cour = fireList;
    while (cour != NULL){
        if (map[(cour->fire).y][(cour->fire).x] == TREE){
            map[(cour->fire).y][(cour->fire).x] = 0;
        }
    }
}