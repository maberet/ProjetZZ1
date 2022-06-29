#include "main.h"

int running;
int game_state;


int main(){ 
    float ***** Q = allocateAndInitiateQ();
    
    srand ( time(NULL));
    

    writeQ(Q);
    running = 1;
    game_state = GAME;
    readMapFromFile("map.txt");
    initPlayer();
    initKeys();
    
    mainLoop();

} 