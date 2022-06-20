#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    SDL_Window
        *window = NULL;

    int window_width = 400;
    int window_height = 300;
    int window_x;
    int window_y;

    SDL_InitSubSystem(SDL_INIT_VIDEO);
    SDL_DisplayMode mode;
    int display_width;
    int display_height;

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
    window_x = (display_width-window_width)/2;
    window_y = display_height;
    window = SDL_CreateWindow(
        "Rocket",
        window_x, window_y,                    // centrage de la fenêtre
        window_width, window_height,           // largeur = 400, hauteur = 300
        SDL_WINDOW_RESIZABLE);                 // redimensionnable

    if (window == NULL)
    {
        SDL_Log("Error : SDL window 1 creation - %s\n",
                SDL_GetError()); // échec de la création de la fenêtre
        SDL_Quit();              // On referme la SDL
        exit(EXIT_FAILURE);
    }



    while(window_y > 0){
        window_y = window_y - 100;
        SDL_SetWindowPosition(window, window_x, window_y);
        SDL_Delay(1000); // Pause exprimée  en ms
    }

    /* et on referme tout ce qu'on a ouvert en ordre inverse de la création */
    SDL_DestroyWindow(window); // la fenêtre

    SDL_Quit(); // la SDL

    return 0;
}