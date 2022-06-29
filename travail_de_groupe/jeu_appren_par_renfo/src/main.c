#include "main.h"

int running;
int game_state;


int main(){
    agent_t * agent; 
    int i ; 
    float ***** Q = allocateAndInitiateQ();
    srand ( time(NULL));
    agent =initAgent(); 
    writeQ(Q);
    running = 1;
    i=setReward( agent->x , agent->y, 0);
    printf("%d \n ",i );
    game_state = GAME;
    readMapFromFile("map.txt");
    initPlayer();
    initKeys();
    
    mainLoop();

} 