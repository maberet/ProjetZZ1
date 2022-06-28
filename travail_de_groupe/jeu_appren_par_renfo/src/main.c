#include "main.h"

int running;
int game_state;


int main(){

    float ***** Q = allocateAndInitiateQ();
    writeQ(Q);
    running = 1;
    game_state = GAME;
    readMapFromFile("map.txt");
    printMap();

    mainLoop();

} 