#include "main.h"

int running;
int game_state;


int main(){
    running = 1;
<<<<<<< HEAD
    game_state = MENU;

    MainLoop();
=======
    game_state = GAME;
    readMapFromFile("map.txt");
    printMap();

    mainLoop();
>>>>>>> main

} 