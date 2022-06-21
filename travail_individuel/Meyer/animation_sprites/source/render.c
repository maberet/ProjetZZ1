#include "render.h"

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Rect rect;

int width = 700;
int height = 700;

void initSDL2(){

    SDL_Init(SDL_INIT_VIDEO);              

    window = SDL_CreateWindow(
        "Animation sprites",                 
        SDL_WINDOWPOS_CENTERED,           
        SDL_WINDOWPOS_CENTERED,           
        width,
        height,
        SDL_WINDOW_RESIZABLE
    );

    if(window == 0){
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError()); 
    }

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    if (renderer == 0) {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError()); 
        SDL_DestroyRenderer(renderer);                                  
        renderer = NULL;
    }

}

void afficherEcran(){
    
    SDL_SetRenderDrawColor(renderer, 0, 16, 158, 0);
    SDL_RenderClear(renderer);

    //

    SDL_RenderPresent(renderer);
}

void destroySDL2(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}