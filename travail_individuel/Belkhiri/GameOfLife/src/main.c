#include "main.h"

int running;
int game_state;


int main(int argc, char** argv){
    running = 1;
    game_state = MENU;

    initMap();

    if(argc == 3){
        game_state = GAME;
        chargeNiveauFichier(argv[2], map, atoi(argv[1]));
        printf("3 arg\n");
    }

    MainLoop();

}