#include "main.h"

int running;
int gameState;


int main(){
    running = 1;
    gameState = GAME;
    readMapFromFile("map.txt");
    mainLoop();
} 