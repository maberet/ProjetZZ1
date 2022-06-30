#include "main.h"

int running;
int game_state;


int main(){ 
    float ***** Q = allocateAndInitiateQ();
    int i= 5;
    
    srand ( time(NULL));
    while (i>0){
    traningAgent(10,1, Q);
    i--;} 
    
    writeQ(Q);
    // running = 1;
    // game_state = GAME;
    // readMapFromFile("map.txt");
    // initPlayer();
    // initKeys();
    
    // mainLoop();

} 