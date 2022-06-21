#include "map.h"


int map[MAPSIZE][MAPSIZE];


void initMap(int map[MAPSIZE][MAPSIZE]) {
    for(int i = 0; i < MAPSIZE; i++){
        for(int j = 0; j < MAPSIZE; j++){
            map[i][j] = 0;
        }
    }
}


void printMap(int map[MAPSIZE][MAPSIZE]){
    for(int i = 0; i < MAPSIZE; i++){
        for(int j = 0; j < MAPSIZE; j++){
            printf("%d ", map[i][j]);
        }
        printf("\n");
    }
}


void writeMap(int map[MAPSIZE][MAPSIZE], char* filename){
    FILE* f = fopen(filename, "w");
    for(int i = 0; i < MAPSIZE; i++){
        for(int j = 0; j < MAPSIZE; j++){
            fprintf(f, "%d ", map[i][j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}