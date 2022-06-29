#include "main.h"

int running;
int game_state;


int main(){ 
    float ***** Q = allocateAndInitiateQ();
    
    srand ( time(NULL));

    traningAgent(1000000, 15, Q);
    

     writeQ(Q);
    // running = 1;
    // game_state = GAME;
    // readMapFromFile("map.txt");
    // initPlayer();
    // initKeys();
    
    // mainLoop();

} 