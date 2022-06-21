#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define TAILLE_MONDE_DELIMITE 50
#define TAILLE_TABLEAU_REGLES 9

int survie[TAILLE_TABLEAU_REGLES] = {0, 0, 1, 1, 0, 0, 0, 0, 0};
int naissance[TAILLE_TABLEAU_REGLES] = {0, 0, 0, 1, 0, 0, 0, 0, 0};

SDL_Window *window;                   
int width = 700;
int height = 700;
SDL_Renderer *renderer;
SDL_Rect rect;
SDL_Event event;
int running = 1;
int tailleAffichage = 30;

void initSDL2(){

    SDL_Init(SDL_INIT_VIDEO);              

    window = SDL_CreateWindow(
        "Jeu de la vie : monde délimité",                 
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
        SDL_DestroyRenderer(renderer);                                  // Attention : on suppose que les NULL sont maintenus !!
        renderer = NULL;
    }

}

void destroySDL2(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void initMonde(int monde[TAILLE_MONDE_DELIMITE][TAILLE_MONDE_DELIMITE]){
    int i = 0;
    int j = 0;
    
    for(i=0; i<TAILLE_MONDE_DELIMITE; i++){
        for(j=0; j<TAILLE_MONDE_DELIMITE; j++){
            monde[i][j] = 0;
        }
    } 
}

void afficheMonde(int monde[TAILLE_MONDE_DELIMITE][TAILLE_MONDE_DELIMITE]){
    int i = 0;
    int j = 0;

    for(i=0; i<TAILLE_MONDE_DELIMITE; i++){
        for(j=0; j<TAILLE_MONDE_DELIMITE; j++){
            switch(monde[i][j]){
                case 0:
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
                    break;
                case 1:
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                    break;
                default:
                    printf("Problème valeur tableau monde.\n");
                    break;
            }
            rect.x = tailleAffichage*i;
            rect.y = tailleAffichage*j;
            rect.w = tailleAffichage-2;
            rect.h = tailleAffichage-2;
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

void afficherEcran(int monde[TAILLE_MONDE_DELIMITE][TAILLE_MONDE_DELIMITE]){
    
    SDL_SetRenderDrawColor(renderer, 0, 16, 158, 0);
    SDL_RenderClear(renderer);

    afficheMonde(monde);

    SDL_RenderPresent(renderer);
}

void changeCellule(int monde[TAILLE_MONDE_DELIMITE][TAILLE_MONDE_DELIMITE], int clic_x, int clic_y){
    int ligne = clic_y/tailleAffichage;
    int colonne = clic_x/tailleAffichage;
    if(0 == monde[colonne][ligne]){
        monde[colonne][ligne] = 1;
    }else{
        monde[colonne][ligne] = 0;
    }
}

int nombreVoisinsVivants(int monde[TAILLE_MONDE_DELIMITE][TAILLE_MONDE_DELIMITE], int cel_x, int cel_y){
    int nbrVoisinsVivants = 0;
    if(cel_x==0 && cel_y==0){
        nbrVoisinsVivants = monde[cel_x+1][cel_y] + monde[cel_x][cel_y+1] + monde[cel_x+1][cel_y+1];
    } else if(cel_x==0 && cel_y==TAILLE_MONDE_DELIMITE-1){
        nbrVoisinsVivants = monde[cel_x+1][cel_y] + monde[cel_x][cel_y-1] + monde[cel_x+1][cel_y-1];
    }else if(cel_x==TAILLE_MONDE_DELIMITE-1 && cel_y==0 ){
        nbrVoisinsVivants = monde[cel_x-1][cel_y] + monde[cel_x][cel_y+1] + monde[cel_x-1][cel_y+1];
    }else if(cel_x==TAILLE_MONDE_DELIMITE-1 && cel_y==TAILLE_MONDE_DELIMITE-1){
        nbrVoisinsVivants = monde[cel_x-1][cel_y] + monde[cel_x][cel_y-1] + monde[cel_x-1][cel_y-1];
    }else if(cel_x==0){
        nbrVoisinsVivants = monde[cel_x][cel_y-1] + monde[cel_x+1][cel_y-1] + monde[cel_x+1][cel_y] + monde[cel_x+1][cel_y+1] + monde[cel_x][cel_y+1];
    }else if(cel_x==TAILLE_MONDE_DELIMITE-1){
        nbrVoisinsVivants = monde[cel_x][cel_y-1] + monde[cel_x-1][cel_y-1] + monde[cel_x-1][cel_y] + monde[cel_x-1][cel_y+1] + monde[cel_x][cel_y+1];
    }else if(cel_y==0){
        nbrVoisinsVivants = monde[cel_x-1][cel_y] + monde[cel_x-1][cel_y+1] + monde[cel_x][cel_y+1] + monde[cel_x+1][cel_y+1] + monde[cel_x+1][cel_y];
    }else if(cel_y==TAILLE_MONDE_DELIMITE-1){
        nbrVoisinsVivants = monde[cel_x-1][cel_y] + monde[cel_x-1][cel_y-1] + monde[cel_x][cel_y-1] + monde[cel_x+1][cel_y-1] + monde[cel_x+1][cel_y];
    }else{
        nbrVoisinsVivants = monde[cel_x-1][cel_y-1] + monde[cel_x][cel_y-1] + monde[cel_x+1][cel_y-1] + monde[cel_x+1][cel_y] +
                            monde[cel_x+1][cel_y+1] + monde[cel_x][cel_y+1] + monde[cel_x-1][cel_y+1] + monde[cel_x-1][cel_y];
    }
    return nbrVoisinsVivants;
}

void reglesEvolutions(int monde[TAILLE_MONDE_DELIMITE][TAILLE_MONDE_DELIMITE]){
    int i = 0;
    int j = 0;
    
    int mondeSuivant[TAILLE_MONDE_DELIMITE][TAILLE_MONDE_DELIMITE] = {0};

    int nbrVoisinsVivants = 0;
    for(i=0; i<TAILLE_MONDE_DELIMITE; i++){
        for(j=0; j<TAILLE_MONDE_DELIMITE; j++){
            nbrVoisinsVivants = nombreVoisinsVivants(monde, i, j);
            if(monde[i][j]==1 && survie[nbrVoisinsVivants]==0){
                mondeSuivant[i][j] = 0;
            }else if(monde[i][j]==0 && naissance[nbrVoisinsVivants]==1){
                mondeSuivant[i][j] = 1;
            }else{
                mondeSuivant[i][j] = monde[i][j];
            }
        }
    }

    for(i=0; i<TAILLE_MONDE_DELIMITE; i++){
        for(j=0; j<TAILLE_MONDE_DELIMITE; j++){
            monde[i][j] = mondeSuivant[i][j];
        }
    }

   
}

int main() {

    int monde[TAILLE_MONDE_DELIMITE][TAILLE_MONDE_DELIMITE];

    initSDL2();

    initMonde(monde);

    while (running) {
        while (SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_WINDOWEVENT:
                    printf("window event\n");
                    switch (event.window.event){
                        case SDL_WINDOWEVENT_CLOSE:  
                            printf("appui sur la croix\n");	
                            break;
                        case SDL_WINDOWEVENT_SIZE_CHANGED:
                            width = event.window.data1;
                            height = event.window.data2;
                            printf("Size : %d%d\n", width, height);
                            break;
                    }   
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    printf("Appui :%d %d\n", event.button.x, event.button.y);
                    changeCellule(monde, event.button.x, event.button.y);
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym){
                        case SDLK_RIGHT:
                            printf("goo calcul!\n");
                            reglesEvolutions(monde);
                            break;
                        case SDLK_s:
                            printf("stop calcul...\n");
                            break;
                        default:
                            printf("une touche est tapee\n");
                            break;
                    }
                    break;
                case SDL_QUIT : 
                    printf("on quitte\n");    
                    running = 0;
                    break;
            }
            //parfois on dessine ici
            afficherEcran(monde);
        }	
        SDL_Delay(10); 
    }
    destroySDL2();
    return 0;
}