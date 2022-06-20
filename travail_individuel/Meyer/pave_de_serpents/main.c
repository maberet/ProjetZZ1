#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    int vainqueur = 0;

    SDL_Window
        *window = NULL;

    int window_width = 1000;
    int window_height = 700;
    int window_x;
    int window_y;

    SDL_InitSubSystem(SDL_INIT_VIDEO);
    SDL_DisplayMode mode;
    int display_width;
    int display_height;

    SDL_Renderer *renderer;
    SDL_Rect rect;

    int pale_1_x;
    int pale_1_y;

    if (SDL_GetDesktopDisplayMode(0, &mode) != 0)
    {
        SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
        return 1;
    }
    display_width = mode.w;
    display_height = mode.h;

    /* Initialisation de la SDL  + gestion de l'échec possible */
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Error : SDL initialisation - %s\n",
                SDL_GetError()); // l'initialisation de la SDL a échoué
        exit(EXIT_FAILURE);
    }

    /* Création de la fenêtre */
    window_x = (display_width - window_width) / 2;
    window_y = (display_height - window_height) / 2;
    window = SDL_CreateWindow(
        "Moulin à serpents",
        window_x, window_y,          // centrage de la fenêtre
        window_width, window_height, // largeur, hauteur
        SDL_WINDOW_RESIZABLE);       // redimensionnable

    if (window == NULL)
    {
        SDL_Log("Error : SDL window 1 creation - %s\n",
                SDL_GetError()); // échec de la création de la fenêtre
        SDL_Quit();              // On referme la SDL
        exit(EXIT_FAILURE);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); /*  SDL_RENDERER_SOFTWARE */
    if (renderer == 0)
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());

        SDL_Quit(); // On referme la SDL
        exit(EXIT_FAILURE);
    }

    // dessin
    /* couleur de fond */
    SDL_SetRenderDrawColor(renderer, 157, 224, 144, 255);
    SDL_RenderClear(renderer);

    //sol
    SDL_SetRenderDrawColor(renderer, 49, 150, 29, 255);
    rect.w = window_width;
    rect.h = window_height/3;
    rect.x = 0;
    rect.y = window_height - window_height/3;
    SDL_RenderFillRect(renderer, &rect);

    /* bloc du moulin */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    rect.w = 150;
    rect.h = 250;
    rect.x = (window_width - rect.w)/2;
    rect.y = window_height - rect.h - 150;
    SDL_RenderFillRect(renderer, &rect);

    /*triangle moulin*/
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    rect.w = 150;
    rect.h = 50;
    rect.x = (window_width - rect.w)/2;
    rect.y = window_height - rect.h - 400;
    SDL_RenderFillRect(renderer, &rect);

    //pales du moulin
    //pale 1 du moulin
    pale_1_x = 50;
    pale_1_y = 50;
    SDL_SetRenderDrawColor(renderer, 163, 82, 2, 255);
    SDL_RenderDrawLine(renderer,
        (window_width - rect.w)/2,
        window_height - rect.h - 400,
        pale_1_x, 
        pale_1_y);

   

    /* afficher à l'ecran */
    SDL_RenderPresent(renderer);



    // départ de la course
    /*while (vainqueur == 0)
    {

        if (cercle.x > display_width)
        {
            vainqueur = 1;
        }
    }*/

    // ecrire victoire


    SDL_Delay(2000);

    // fermer fenetre
    SDL_DestroyWindow(window); // la fenêtre

    SDL_Quit(); // la SDL

    return 0;
}