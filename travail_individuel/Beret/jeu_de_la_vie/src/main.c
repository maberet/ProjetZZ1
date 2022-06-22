#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "map.h"
#include "render.h"



int main (int argc, char **argv) {
    
    if (argc=! 2){
        printf("trop/peu d'argument \n");
        exit(0);
    }
    SDL_Window 
       *window_1 = NULL;
    SDL_Renderer
        *renderer = NULL;
    SDL_bool
        program_on= SDL_TRUE; 

    int dimension_grille= atoi(argv[1]);
    int dimension_tableau=600-600%dimension_grille;
    int regles_naissances [9]={0,0,0,1,0,0,0,0,0};
    int regles_survie [9]={0,0,1,1,0,0,0,0,0};
    int **grille=NULL;
    int i;
    int taille_max=20;
    

        
        init_sdl(window_1,renderer,dimension_tableau);

        grille = malloc(dimension_grille*sizeof(int));
        if (grille==NULL){
            fin_sdl(renderer,window_1);
            printf("erreur alloc\n");
            exit(0);
        }
        else{
            for (i=0;i<dimension_grille; i++){
                grille[i]=malloc(dimension_grille*sizeof(int));
                if (grille[i]==NULL){
                    fin_sdl(renderer,window_1);
                    liberer_grille(grille,i-1);
                    printf("erreur alloc\n");
                    exit(0);
                }
            }
        }

        int ** nouvelle_grille=NULL;

        nouvelle_grille = malloc(dimension_grille*sizeof(int));
        if (nouvelle_grille==NULL){
            fin_sdl(renderer,window_1);
            printf("erreur alloc\n");
            exit(0);
        }
        else{
            for (i=0;i<dimension_grille; i++){
                nouvelle_grille[i]=malloc(dimension_grille*sizeof(int));
                if (nouvelle_grille[i]==NULL){
                    fin_sdl(renderer,window_1);
                    liberer_grille(nouvelle_grille,i-1);
                    printf("erreur alloc\n");
                    exit(0);
                }
            }
        }
        init_grille(grille,dimension_grille);
        printf("dim%d\n",grille[20][20]);

        SDL_Event event;     
        
        dessiner_grille(renderer,grille, dimension_grille,dimension_tableau);
        SDL_RenderPresent(renderer);                          

        while (program_on) {                              // boucle d'event 
            while(SDL_PollEvent(&event)) {   
                                                    
                switch (event.type) {
                    case SDL_QUIT:
                        program_on=SDL_FALSE;
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        printf("Appui :%d %d\n", event.button.x, event.button.y);
                        changer_case(grille, event.button.x, event.button.y,dimension_grille);
                        break;
                    case SDL_KEYDOWN:
                        switch(event.key.keysym.sym){
                            case SDLK_RIGHT:
                                printf("goo calcul!\n");
                                iteration_suivante(grille,nouvelle_grille,regles_naissances,regles_survie,dimension_grille);
                                break;
                            default:
                                printf("une touche est tapee\n");
                                break;
                        }
                        break;
                        case SDL_WINDOWEVENT_CLOSE:  
                            printf("appui sur la croix\n");	
                            break;
                
                }
            dessiner_grille(renderer,grille, dimension_grille,dimension_tableau);

            SDL_RenderPresent(renderer);
            }      
        SDL_Delay(100);
        }
        
        liberer_grille(grille, dimension_grille);
        liberer_grille(nouvelle_grille,dimension_grille);

        fin_sdl(renderer,window_1);

    return 0; 
}