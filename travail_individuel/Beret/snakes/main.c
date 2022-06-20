#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    SDL_Window 
       *window_1 = NULL;
    SDL_Renderer
        *renderer = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("Error : SDL initialisation - %s\n", 
             SDL_GetError());                // l'initialisation de la SDL a échoué 
    exit(EXIT_FAILURE);}

    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);

    /* Création de la fenêtre de gauche */
    window_1 = SDL_CreateWindow("Premier dessin",
                            SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, DM.w * 0.66,
                            DM.h * 0.66,
                            SDL_WINDOW_OPENGL);              

    if (window_1 == NULL) {
        SDL_Log("Error : SDL window 1 creation - %s\n", 
                SDL_GetError());                 // échec de la création de la fenêtre
        SDL_Quit();                              // On referme la SDL       
        exit(EXIT_FAILURE);
    }

    renderer = SDL_CreateRenderer(window_1, -1,
                                SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {SDL_DestroyRenderer(renderer);                                  // Attention : on suppose que les NULL sont maintenus !!
    renderer = NULL;

    SDL_DestroyWindow(window_1);
    window_1 = NULL;};

    //draw(renderer);                                       
    SDL_RenderPresent(renderer);
    SDL_Delay(2000); 
    SDL_DestroyRenderer(renderer);                                  // Attention : on suppose que les NULL sont maintenus !!
    renderer = NULL;

    SDL_DestroyWindow(window_1);
    window_1 = NULL;
    SDL_Quit();                                

    return 0;
}
