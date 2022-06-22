#include "main.h"

int running;
int gameState;


int main(){
    running = 1;
    gameState = MENU;
    readMapFromFile("map.txt");
    fireList = startFire(fireList,10,MAPSIZE);
    mainLoop();
} 