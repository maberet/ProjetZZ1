#include "main.h"

int running = 1;

int main(int argc, char** argv) {

    initSDL2();

    loadTexture();

    while (running) {
        eventLoop();
        afficherEcran();
    }

    destroySDL2();

    return 0;
}