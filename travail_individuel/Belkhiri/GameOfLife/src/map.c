#include "map.h"

int MAPSIZE = 20;

int ** map;

void allocateMap(){
    map = malloc(MAPSIZE * sizeof(int*));
    for(int i = 0; i < MAPSIZE; i++){
        map[i] = malloc(MAPSIZE * sizeof(int));
    }
}


void initMap() {
    allocateMap();
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