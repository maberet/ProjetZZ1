#include "map.h"


int map[MAPSIZE][MAPSIZE];


void initMap(int map[MAPSIZE][MAPSIZE]) {
    for(int i = 0; i < MAPSIZE; i++){
        for(int j = 0; j < MAPSIZE; j++){
            map[i][j] = rand() % 2;
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