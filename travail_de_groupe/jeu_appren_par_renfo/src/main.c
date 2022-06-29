#include "main.h"

int running;
int game_state;


int main(){ 
    float ***** Q = allocateAndInitiateQ();
    int i = 10000;
    
    srand ( time(NULL));
    while (i>0){
    traningAgent(10000000, 40, Q);
    i--;} 
    
     writeQ(Q);
    // running = 1;
    // game_state = GAME;
    // readMapFromFile("map.txt");
    // initPlayer();
    // initKeys();
    
    // mainLoop();

} 