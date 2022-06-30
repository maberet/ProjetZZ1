#include "main.h"

int running;
int game_state;
float ***** Q;
int main(){ 
    float ***** Q = allocateAndInitiateQ();
    //int i= 10;
    
    srand ( time(NULL));
    // while (i>0){
    traningAgent(10000,30, Q);
    // i--;} 
    //printf(" %d \n ", convertIntoZone(agent->x,agent->y));
    
    writeQ(Q);
    // running = 1;
    // game_state = GAME;
    // readMapFromFile("map.txt");
    // initPlayer();
    // initKeys();
    
    // mainLoop();


    //mainLoop();
}
