#include "main.h"

int running;
int game_state;
float ***** Q;
int main(){ 
    Q = allocateAndInitiateQ();
    readQFromFile(Q);
    int i= 50;
    
    srand ( time(NULL));
    while (i>0){
    traningAgent(1000000,30, Q);
    printf("%d \n ", i);
    writeQ(Q);
    i--;} 
    
    writeQ(Q);
    // running = 1;
    // game_state = GAME;
    // readMapFromFile("map.txt");
    // initPlayer();
    // initKeys();
    
    // mainLoop();


    mainLoop();
}
