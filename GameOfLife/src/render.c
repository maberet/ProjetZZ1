#include "render.h"

SDL_Window *window;
SDL_Renderer *renderer;

TTF_Font *RobotoFont;

void CreateWindow(){
    SDL_DisplayMode ScreenDimension;

    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        printf("Couldn't create window.");
        exit(EXIT_FAILURE);
    }

    SDL_GetCurrentDisplayMode(0, &ScreenDimension);

    window = SDL_CreateWindow("Game Of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ScreenDimension.w, ScreenDimension.h, SDL_WINDOW_INPUT_GRABBED | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);

    if (window == NULL){
        printf("Couldn't create window");
        exit(EXIT_FAILURE);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL){
        printf("Couldn't create renderer.");
        exit(EXIT_FAILURE);
    }

    if (TTF_Init() == -1)
    {
        exit(EXIT_FAILURE);
    }

    RobotoFont = TTF_OpenFont("Res/Roboto-Black.ttf", 50);  

}


void MainLoop(){
    CreateWindow();

    unsigned int a = SDL_GetTicks();
    unsigned int b = SDL_GetTicks();
    double delta = 0;

    pthread_t eventThread;
    if (pthread_create(&eventThread, NULL, EventLoop, NULL) != 0){
        printf("Couldn't create thread.");
        exit(EXIT_FAILURE);
    }

    while (running){
        a = SDL_GetTicks();
        delta = (a - b) / 1000.0;
        if (delta > 1/FPS_TO_GET){
            b = a;
            switch (game_state){
                case MENU:
                    //Menu();
                    break;
                case GAME:
                    //Game();
                    break;
            }
        }
        else {
            // fait dormir le thread pour garder des ressources
            usleep(1000 * (1/FPS_TO_GET - delta));
        }

    }
}