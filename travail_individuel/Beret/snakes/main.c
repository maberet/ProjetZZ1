#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>



void draw(SDL_Renderer* renderer, int temps) {                               
  SDL_Rect rectangle = {0,0,300,200};   
  SDL_Rect rectangle2 = {(temps/2)%(300),150,20,10};
  SDL_Rect rectangle3 = {(temps/3+50)%(300),10,35,15};
  SDL_Rect rectangle4 = {(temps/2+150)%(300),190,50,20};
  int oiseauxx[17]={0,1,2,2,2,3,3,4,1,1,0,0,0,-1,-1,-2,-3};
  int oiseauxy[17]={0,0,0,1,2,2,3,3,-1,-2,-1,-2,-3,0,1,1,2};

  srand( time( NULL ) );
  int saut= rand() % 10;
  srand( time( NULL ) );
  int signe= rand() % 2;


  
  SDL_SetRenderDrawColor(renderer,                                                
                         128, 196, 255,                                  // mode Red, Green, Blue (tous dans 0..255)
                         255);                                           // 0 = transparent ; 255 = opaque

  SDL_RenderFillRect(renderer, &rectangle);


  SDL_SetRenderDrawColor(renderer,                                                
                         255, 255, 255,                                  // mode Red, Green, Blue (tous dans 0..255)
                         150);                                           // 0 = transparent ; 255 = opaque

  SDL_RenderFillRect(renderer, &rectangle2);

  SDL_SetRenderDrawColor(renderer,                                                
                         255, 255, 255,                                  // mode Red, Green, Blue (tous dans 0..255)
                         150);                                           // 0 = transparent ; 255 = opaque

  SDL_RenderFillRect(renderer, &rectangle3);

  SDL_SetRenderDrawColor(renderer,                                                
                         255, 255, 255,                                  // mode Red, Green, Blue (tous dans 0..255)
                         150);                                           // 0 = transparent ; 255 = opaque

  SDL_RenderFillRect(renderer, &rectangle4);

  if (signe%2!=0){saut = -saut; }
  int pt = 0;
  /* tracer un cercle n'est en fait pas trivial, voilà le résultat sans algo intelligent ... */
  while ( pt<16) {      
    SDL_SetRenderDrawColor(renderer,
                           0,          // quantité de Rouge      
                           0,          //          de vert 
                           0,              //          de bleu
                           255);
                                                              // opacité = opaque
    SDL_RenderDrawPoint(renderer,                                   
                        (200+ temps- oiseauxx[pt])%(300),                     // coordonnée en x
                        100-oiseauxy[pt]+saut/2);
    
    SDL_SetRenderDrawColor(renderer,
                           0,          // quantité de Rouge      
                           0,          //          de vert 
                           0,              //          de bleu
                           255);
                                                              // opacité = opaque
    SDL_RenderDrawPoint(renderer,                                   
                        (180+ temps - oiseauxx[pt])%(300),                     // coordonnée en x
                        150-oiseauxy[pt]-(1.5*saut));
    SDL_SetRenderDrawColor(renderer,
                           0,          // quantité de Rouge      
                           0,          //          de vert 
                           0,              //          de bleu
                           255);
                                                              // opacité = opaque
    SDL_RenderDrawPoint(renderer,                                   
                        (10+ temps - oiseauxx[pt])%(300),                     // coordonnée en x
                        10-oiseauxy[pt]-(saut));
    
    pt=pt+1;}
}

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;
    int temps=0;


    SDL_Window 
       *window_1 = NULL;
    SDL_Renderer
        *renderer = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("Error : SDL initialisation - %s\n", 
             SDL_GetError());                // l'initialisation de la SDL a échoué 
    exit(EXIT_FAILURE);}

    window_1 = SDL_CreateWindow("Premier dessin",
                            SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, 300,
                            200,
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
    while (temps<1000){
    //draw(renderer);  
    draw (renderer,temps);                                     
    SDL_RenderPresent(renderer);
    SDL_Delay(20);
    temps= temps +1;  }

    SDL_DestroyRenderer(renderer);                                  // Attention : on suppose que les NULL sont maintenus !!
    renderer = NULL;

    SDL_DestroyWindow(window_1);
    window_1 = NULL;
    SDL_Quit();                                

    return 0;
}
