#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  SDL_Window 
       *window = NULL;

  /* Initialisation de la SDL  + gestion de l'échec possible */
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("Error : SDL initialisation - %s\n", 
             SDL_GetError());                // l'initialisation de la SDL a échoué 
    exit(EXIT_FAILURE);
  }

  /* Création de la fenêtre */
  window = SDL_CreateWindow(
      "Rocket",                    
      0, 0,                                  // coin haut gauche en haut gauche de l'écran
      400, 300,                              // largeur = 400, hauteur = 300
      SDL_WINDOW_RESIZABLE);                 // redimensionnable

  if (window == NULL) {
    SDL_Log("Error : SDL window 1 creation - %s\n", 
            SDL_GetError());                 // échec de la création de la fenêtre
    SDL_Quit();                              // On referme la SDL       
    exit(EXIT_FAILURE);
  }

  /* Normalement, on devrait ici remplir les fenêtres... */
  SDL_Delay(2000);                           // Pause exprimée  en ms

  /* et on referme tout ce qu'on a ouvert en ordre inverse de la création */
  SDL_DestroyWindow(window);               // la fenêtre   

  SDL_Quit();                                // la SDL

  return 0;
}