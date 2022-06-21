#include "utility.h"

void end_sdl(char ok,                                               // fin normale : ok = 0 ; anormale ok = 1
             char const* msg,                                       // message à afficher
             SDL_Window* window,                                    // fenêtre à fermer
             SDL_Renderer* renderer) {                              // renderer à fermer
  char msg_formated[255];                                                         
  int l;                                                                          

  if (!ok) {                                                        // Affichage de ce qui ne va pas
    strncpy(msg_formated, msg, 250);                                              
    l = strlen(msg_formated);                                                     
    strcpy(msg_formated + l, " : %s\n");                                          

    SDL_Log(msg_formated, SDL_GetError());                                        
  }                                                                               

  if (renderer != NULL) {                                           // Destruction si nécessaire du renderer
    SDL_DestroyRenderer(renderer);                                  // Attention : on suppose que les NULL sont maintenus !!
    renderer = NULL;
  }
  if (window != NULL)   {                                           // Destruction si nécessaire de la fenêtre
    SDL_DestroyWindow(window);                                      // Attention : on suppose que les NULL sont maintenus !!
    window= NULL;
  }

  SDL_Quit();                                                                     

  if (!ok) {                                                        // On quitte si cela ne va pas                
    exit(EXIT_FAILURE);                                                           
  }                                                                               
}   