#include "render.h"

SDL_Window *window; 
SDL_Renderer *renderer; 

SDL_Surface *oursSurface=NULL;
SDL_Texture *oursTexture=NULL; 

SDL_Surface * fondSurface1=NULL;
SDL_Texture * fondTexture1=NULL;

SDL_Surface * fondSurface2=NULL;
SDL_Texture * fondTexture2=NULL;

SDL_Surface * fondSurface3=NULL;
SDL_Texture * fondTexture3=NULL;

void creation_fen(){

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("Error : SDL initialisation - %s\n", 
             SDL_GetError());                // l'initialisation de la SDL a échoué 
    exit(EXIT_FAILURE);}

        window = SDL_CreateWindow("Running Bear", 
                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 180,SDL_WINDOW_OPENGL);
           

    if (window == NULL) {
        SDL_Log("Error : SDL window 1 creation - %s\n", 
                SDL_GetError());                 // échec de la création de la fenêtre
        SDL_Quit();                              // On referme la SDL       
        exit(EXIT_FAILURE);
    }

    renderer = SDL_CreateRenderer(window, -1,
                                SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {SDL_DestroyRenderer(renderer);                                  // Attention : on suppose que les NULL sont maintenus !!
    renderer = NULL;

    SDL_DestroyWindow(window);
    window = NULL;};

}

void dessin_fond(int temps,int w , int h){
    SDL_Rect rect2 = {0, 0, w,h};
    SDL_Rect destRect2 = {temps%320, 0,w, h};
    SDL_RenderCopyEx(renderer,fondTexture1, &destRect2, &rect2, 0, NULL, SDL_FLIP_NONE);
    SDL_Rect rect3 = {0, 0, w,h};
    SDL_Rect destRect3 = {temps%320, 0,w, h};
    SDL_RenderCopyEx(renderer,fondTexture2, &destRect3, &rect3, 0, NULL, SDL_FLIP_NONE);
    SDL_Rect rect4 = {0, 0, w,h};
     SDL_Rect destRect4 = {temps%320, 0,w, h};
    SDL_RenderCopyEx(renderer,fondTexture3, &destRect4, &rect4, 0, NULL, SDL_FLIP_NONE);
}

void dessin_ours(int temps){
    SDL_Rect rect = {(-64+10*temps)%(640-64), 52, 128,128};
    SDL_Rect destRect = {128 * (SDL_GetTicks()/200%8), 0,128, 128};
    SDL_RenderCopyEx(renderer,oursTexture, &destRect, &rect, 0, NULL, SDL_FLIP_NONE);
}


void dessin (int temps,int w,int h) {
    SDL_RenderClear(renderer);
    dessin_fond(temps,w,h);
    dessin_ours(temps);
    SDL_RenderPresent(renderer);
    SDL_Delay(100); 
}

void affichage () {
    int i=0;
    int w=320;
    int h=180;
    int running =1;
    creation_fen();

    oursSurface= IMG_Load("assets/Sprite-0001.png");
    oursTexture = SDL_CreateTextureFromSurface(renderer, oursSurface);

    fondSurface1=IMG_Load("assets/DarkForest_Background.png");
    fondTexture1 = SDL_CreateTextureFromSurface(renderer, fondSurface1);

    fondSurface2=IMG_Load("assets/DarkForest_Middleground.png");
    fondTexture2 = SDL_CreateTextureFromSurface(renderer, fondSurface2);

    fondSurface3=IMG_Load("assets/DarkForest_Foreground.png");
    fondTexture3 = SDL_CreateTextureFromSurface(renderer, fondSurface3);

    SDL_QueryTexture(fondTexture1,NULL,NULL,&w,&h);
    printf("w:%d,h:%d\n",w,h);

    SDL_FreeSurface(oursSurface);
    
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)){
                switch(event.type)
                {   
                    case SDL_QUIT:
                        running = 0;
                        break;
                    default:
                        continue;
                }
        }
        dessin(i,w,h);
        i++;  
                            
    }
    SDL_DestroyRenderer(renderer);                                  // Attention : on suppose que les NULL sont maintenus !!
    renderer = NULL;
    SDL_DestroyWindow(window);
    window=NULL;
    SDL_Quit();         
    
}
