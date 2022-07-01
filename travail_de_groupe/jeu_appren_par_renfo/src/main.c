
#include "main.h"

int running;
int game_state;
float ***** Q;
int main()
{
    srand(time(NULL));
    Q = allocateAndInitiateQ();
    readQFromFile(Q);
    //traningAgent(10000, 30 , Q);
    running = 1;
    game_state = GAME;
    readMapFromFile("map.txt");

    initPlayer();
    initEnnemy();
    initKeys();
    initBall();
    freeQ(Q); 


    mainLoop();
}