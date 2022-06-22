#include "main.h"

int running;
int gameState;


int main(){
    running = 1;
    gameState = MENU;
    readMapFromFile("map.txt");
    initPlayer();

    mainLoop();

} 