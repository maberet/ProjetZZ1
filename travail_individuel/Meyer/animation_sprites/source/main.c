#include "main.h"

int running = 1;

int main(int argc, char** argv) {

    initSDL2();
    
    /*pthread_t eventThread;
    if (pthread_create(&eventThread, NULL, eventLoop, NULL) != 0){
        printf("Couldn't create thread.");
        exit(EXIT_FAILURE);
    }*/

    while (running) {
        eventLoop();
        afficherEcran();
    }

    destroySDL2();

    return 0;
}