#include "main.h"

int running;
int game_state;

int main()
{
    running = 1;
    game_state = GAME;
    readMapFromFile("map.txt");

    initPlayer();
    initKeys();
    initBall();

    mainLoop();
}