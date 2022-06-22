#include "render.h"

void init_sdl(SDL_Window *window_1,SDL_Renderer *renderer,int dimension_tableau){
   
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("Error : SDL initialisation - %s\n", 
             SDL_GetError());                // l'initialisation de la SDL a échoué 
    exit(EXIT_FAILURE);}

    window_1 = SDL_CreateWindow("Jeu de la vie",
                            SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, dimension_tableau,
                            dimension_tableau,
                            SDL_WINDOW_RESIZABLE);              

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

}

void dessiner_grille(SDL_Renderer* renderer, int **grille,int taille, int taille_fen){

    int i;
    int j; 
    int taille_case= taille_fen/taille;
    SDL_RenderClear(renderer);
    for (i=0;i<taille;i++){
        for(j=0;j<taille;j++){

            SDL_Rect rectangle;
            rectangle.x = quelleTaillePourLesCases(width, taille_monde_delimitee)*i;
            rectangle.y = quelleTaillePourLesCases(height, taille_monde_delimitee)*j;
            rectangle.w = quelleTaillePourLesCases(width, taille_monde_delimitee)-2;
            rectangle.h = quelleTaillePourLesCases(height, taille_monde_delimitee)-2;

            if (grille [i][j]==1){
                SDL_SetRenderDrawColor(renderer,                                                
                         0, 0, 0,                                  // mode Red, Green, Blue (tous dans 0..255)
                         255); 
                SDL_RenderFillRect(renderer, &rectangle);}
           
            else{
                SDL_SetRenderDrawColor(renderer,                                                
                         255, 255, 255,                                  // mode Red, Green, Blue (tous dans 0..255)
                         255); 
                SDL_RenderFillRect(renderer, &rectangle);
            }
        }
    }
    SDL_RenderPresent(renderer);
}

void fin_sdl(SDL_Renderer* renderer,SDL_Window *window_1){    
    SDL_DestroyRenderer(renderer);                               
    renderer = NULL;

    SDL_DestroyWindow(window_1);
    window_1 = NULL;
    SDL_Quit(); 
}