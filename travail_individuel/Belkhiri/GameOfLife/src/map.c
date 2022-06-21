#include "map.h"

int MAPSIZE = 20;

int ** map;

int *** mapList;

void allocateMap(){
    map = malloc(MAPSIZE * sizeof(int*));
    for(int i = 0; i < MAPSIZE; i++){
        map[i] = malloc(MAPSIZE * sizeof(int));
    }
}

void allocateMapList(int SIZE){
    mapList = malloc(SIZE * sizeof(int**));
}


void initMap() {
    allocateMap();
    allocateMapList(100);
    for(int i = 0; i < MAPSIZE; i++){
        for(int j = 0; j < MAPSIZE; j++){
            map[i][j] = 0;
        }
    }
}


void printMap(){
    for(int i = 0; i < MAPSIZE; i++){
        for(int j = 0; j < MAPSIZE; j++){
            printf("%d ", map[i][j]);
        }
        printf("\n");
    }
}


void writeMap(char* filename){
    FILE* f = fopen(filename, "w");
    for(int i = 0; i < MAPSIZE; i++){
        for(int j = 0; j < MAPSIZE; j++){
            fprintf(f, "%d ", map[i][j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

int ** saveMapState(){
    int ** mapCopy = malloc(MAPSIZE * sizeof(int*));
    for(int i = 0; i < MAPSIZE; i++){
        mapCopy[i] = malloc(MAPSIZE * sizeof(int));
    }
    for(int i = 0; i < MAPSIZE; i++){
        for(int j = 0; j < MAPSIZE; j++){
            mapCopy[i][j] = map[i][j];
        }
    }
    return mapCopy;
}

void addMapToList(int ** map){
    int i = 0;
    while(mapList[i] != NULL){
        i++;
    }
    mapList[i] = map;
}

void checkForCycle(){
    int i = 0;
    while(mapList[i] != NULL){
        if(mapList[i] == map){
            printf("Cycle detected at iteration: %d\n", i);
            return;
        }
        i++;
    }
}