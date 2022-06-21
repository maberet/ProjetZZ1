#include "main.h"

int running;
int game_state;


int main(){
    running = 1;
    game_state = MENU;
    initMap(map);

    MainLoop();

}