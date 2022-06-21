#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

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

    int number_of_stars = 10;

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
    window_x = (display_width - window_width) / 2;
    window_y = display_height;
    window = SDL_CreateWindow(
        "Rocket : 3",
        window_x, window_y,          // centrage de la fenêtre
        window_width, window_height, // largeur = 400, hauteur = 300
        SDL_WINDOW_RESIZABLE);       // redimensionnable

    if (window == NULL)
    {
        SDL_Log("Error : SDL window 1 creation - %s\n",
                SDL_GetError()); // échec de la création de la fenêtre
        SDL_Quit();              // On referme la SDL
        exit(EXIT_FAILURE);
    }

    // decompte decollage
    SDL_Delay(1000);
    SDL_SetWindowTitle(window, "Rocket : 2");
    SDL_Delay(1000);
    SDL_SetWindowTitle(window, "Rocket : 1");
    SDL_Delay(1000);
    SDL_SetWindowTitle(window, "Rocket : FEU!");
    SDL_Delay(1000);
    SDL_SetWindowTitle(window, "Rocket");

    while (window_y > 0)
    {
        window_width = window_width - 15;
        window_x = (display_width - window_width) / 2;
        window_y = window_y - 100;
        SDL_SetWindowSize(window, window_width, window_height);
        SDL_SetWindowPosition(window, window_x, window_y);
        SDL_Delay(200); // Pause exprimée  en ms
    }

    // explosion de la rocket
    SDL_DestroyWindow(window); // la fenêtre

    // aleatoire
    srand(time(0));
    // creation de [number_of_stars] etoiles dans des directions aleatoires
    SDL_Window *window_stars[20];
    int tab_window_x[20];
    int tab_window_y[20];
    int tab_direction[20];
    for (int i = 1; i <= number_of_stars; i++)
    {
        int width_r = 50 + rand() % 100;
        int height_r = 50 + rand() % 50;
        tab_window_x[i] = (display_width - width_r) / 2;
        tab_window_y[i] = 0;
        tab_direction[i] = pow(-1, 1+(rand()%2)); // donne -1 ou 1

        window_stars[i] = SDL_CreateWindow(
            "Étoile",
            tab_window_x[i], tab_window_y[i],
            width_r, height_r,
            SDL_WINDOW_RESIZABLE);

        SDL_Delay(10); // Pause exprimée  en ms
    }

    // déplacement des etoiles
    for (int temps = 0; temps <= 5; temps++)
    {
        for (int i = 1; i <= number_of_stars; i++)
        {
            tab_window_y[i] = tab_window_y[i] + 20 + rand()%70;
            tab_window_x[i] = tab_window_x[i] + tab_direction[i]*(rand()%200);
            SDL_SetWindowPosition(window_stars[i], tab_window_x[i], tab_window_y[i]);
        }
        SDL_Delay(400);
    }

    // destruction des etoiles
    for (int i = 1; i <= number_of_stars; i++)
    {
        SDL_DestroyWindow(window_stars[i]);
    }

    SDL_Quit(); // la SDL

    return 0;
}