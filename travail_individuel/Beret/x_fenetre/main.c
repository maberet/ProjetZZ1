#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;
  int x=0;

  

  SDL_Window 
       *window_1 = NULL,*window_2=NULL, *window_3=NULL,*window_4=NULL;                     // Future fenêtre de gauche
 
  /* Initialisation de la SDL  + gestion de l'échec possible */
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("Error : SDL initialisation - %s\n", 
             SDL_GetError());                // l'initialisation de la SDL a échoué 
    exit(EXIT_FAILURE);
  }
  
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    int Width = DM.w;

    /* Création de la fenêtre de gauche */
    window_1 = SDL_CreateWindow(
        "Vague",                    // codage en utf8, donc accents possibles
        x,300 + 100*cos(x/10),                                  
        100,100,                               
        SDL_WINDOW_RESIZABLE);                 

    if (window_1 == NULL) {
        SDL_Log("Error : SDL window 1 creation - %s\n", 
                SDL_GetError());                 // échec de la création de la fenêtre
        SDL_Quit();                              // On referme la SDL       
        exit(EXIT_FAILURE);
    }
     window_2 = SDL_CreateWindow(
      "vague2",                    // codage en utf8, donc accents possibles
      x-10,300 + 110*cos((x-10)/10),                                // à droite de la fenêtre de gauche
      80, 80,                              // largeur = 500, hauteur = 300
      0);

  if (window_2 == NULL) {
    SDL_Log("Error : SDL window 2 creation - %s\n", 
            SDL_GetError());                 // échec de la création de la deuxième fenêtre 
    SDL_DestroyWindow(window_1);             // la première fenétre (qui elle a été créée) doit être détruite
    SDL_Quit();
    exit(EXIT_FAILURE);
  }
    window_3 = SDL_CreateWindow(
      "vague3",                    // codage en utf8, donc accents possibles
      x-20,300 + 130*cos((x-20)/10),                                // à droite de la fenêtre de gauche
      60, 60,                              // largeur = 500, hauteur = 300
      0);

  if (window_3 == NULL) {
    SDL_Log("Error : SDL window 3 creation - %s\n", 
            SDL_GetError());                 // échec de la création de la deuxième fenêtre 
    SDL_DestroyWindow(window_2); 
    SDL_DestroyWindow(window_1);             // la première fenétre (qui elle a été créée) doit être détruite
    SDL_Quit();
    exit(EXIT_FAILURE);
  }

  window_4 = SDL_CreateWindow(
      "vague4",                    // codage en utf8, donc accents possibles
      x-30,300 + 150*cos((x-30)/10),                                // à droite de la fenêtre de gauche
      40, 40,                              // largeur = 500, hauteur = 300
      0);

  if (window_4 == NULL) {
    SDL_Log("Error : SDL window 4 creation - %s\n", 
            SDL_GetError());                 // échec de la création de la deuxième fenêtre 
    SDL_DestroyWindow(window_3); 
    SDL_DestroyWindow(window_2); 
    SDL_DestroyWindow(window_1);             // la première fenétre (qui elle a été créée) doit être détruite
    SDL_Quit();
    exit(EXIT_FAILURE);
  }
  while (x<Width){
    SDL_SetWindowPosition(window_1,x,300 + 100*cos((x)/10));
    SDL_SetWindowPosition(window_2,x-10,300 + 100*cos((x-10)/10));
    SDL_SetWindowPosition(window_3,x-20,300 + 100*cos((x-20)/10));
    SDL_SetWindowPosition(window_4,x-30,300 + 100*cos((x-30)/10));
    x=x+10;
    
    SDL_Delay(150);                           // Pause exprimée  en ms
    }   

    SDL_DestroyWindow(window_4); 
    SDL_DestroyWindow(window_3); 
    SDL_DestroyWindow(window_2);               
    SDL_DestroyWindow(window_1);                

  SDL_Quit();                                // la SDL

  return 0;
}