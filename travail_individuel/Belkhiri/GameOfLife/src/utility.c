#include "utility.h"

int surviveRule[NB_RULES] = {0, 0, 1, 1, 0, 0, 0, 0, 0};
int bornRule[NB_RULES] = {0, 0, 0, 1, 0, 0, 0, 0, 0};
int stable;

long long int BinaryList[LIST_SIZE];
int BinaryListSize;

int survivingNeighbors(int x, int y){
    int count = 0;
    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){
            if(i == 0 && j == 0) continue;
            if(map[(x+i+MAPSIZE)%MAPSIZE][(y+j+MAPSIZE)%MAPSIZE] == 1) count++;
        }
    }
    return count;
}

void addBinaryToList(long long int binary){
    if (BinaryListSize == LIST_SIZE) return;
    BinaryList[BinaryListSize] = binary;
    BinaryListSize++;
}



void printBinaryList(){
    for (int i=0; i<BinaryListSize; i++){
        printf("%lld ", BinaryList[i]);
    }
    printf("\n");
}


void checkForCycle(){
    int i = 0;
    printBinaryList();
    while (i < BinaryListSize-1){
        if (BinaryList[i] == BinaryList[BinaryListSize - 1]){
            printf("%lld\n %lld\n", BinaryList[i], BinaryList[BinaryListSize - 1]);
            printf("Cycle detected!\n");
            return;
        }
        i++;
    }
}


void updateMap(){
    int newMap[MAPSIZE][MAPSIZE];
    stable = 1;

    long long int binary = MapToBinary(map);
    addBinaryToList(binary);

    for (int i=0; i<MAPSIZE; i++){
        for (int j=0; j<MAPSIZE; j++){
            int count = survivingNeighbors(j, i);
            if (map[j][i] == 1){
                if (!surviveRule[count]) newMap[j][i] = 0;
                else newMap[j][i] = 1;
            }
            else{
                if (bornRule[count]) newMap[j][i] = 1;
                else newMap[j][i] = 0;
            }
        }
    }

    for (int i=0; i<MAPSIZE; i++){
        for (int j=0; j<MAPSIZE; j++){
            if (newMap[j][i] != map[j][i]) stable = 0;
        }
    }

    for (int i=0; i<MAPSIZE; i++){
        for (int j=0; j<MAPSIZE; j++){
            map[i][j] = newMap[i][j];
        }
    }

    checkForCycle();
}

