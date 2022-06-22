#include "render.h"

SDL_Window *window;
SDL_Renderer *renderer;

TTF_Font *robotoFont;
SDL_DisplayMode screenDimension;

SDL_Surface * grassSurface;
SDL_Texture * grassTexture;

SDL_Surface * treeSurface;
SDL_Texture * treeTexture;

SDL_Surface * hoverSurface;
SDL_Texture * hoverTexture;

SDL_Surface * playerSurface;
SDL_Texture * playerTexture;

SDL_Surface * backgroundSurface;
SDL_Texture * backgroundTexture;

void createWindow(){

    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        printf("Couldn't create window.");
        exit(EXIT_FAILURE);
    }

    SDL_GetCurrentDisplayMode(0, &screenDimension);

    window = SDL_CreateWindow("Game Of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenDimension.w, screenDimension.h, SDL_WINDOW_INPUT_GRABBED | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);

    if (window == NULL){
        printf("Couldn't create window");
        exit(EXIT_FAILURE);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL){
        printf("Couldn't create renderer.");
        exit(EXIT_FAILURE);
    }

    if (TTF_Init() == -1)
    {
        exit(EXIT_FAILURE);
    }

    robotoFont = TTF_OpenFont("Res/Roboto-Black.ttf", 50);  

}

void drawMap(){
    int i, j;
    SDL_Rect rect;
    rect.h = CELLSIZE;
    rect.w = CELLSIZE;
    rect.x = (screenDimension.w - (MAPSIZE * CELLSIZE)) / 2; // centers the drawing
    rect.y = 0;
    for(i = 0; i < MAPSIZE; i++){
        for(j = 0; j < MAPSIZE; j++){
            SDL_RenderCopy(renderer, grassTexture, NULL, &rect);
            if(map[i][j] == 1){
                SDL_RenderCopy(renderer, treeTexture, NULL, &rect);
            }
            if (hover[0] == j && hover[1] == i){
                SDL_RenderCopy(renderer, hoverTexture, NULL, &rect);
            }
            rect.x += CELLSIZE;
        }
        rect.x = (screenDimension.w - (MAPSIZE * CELLSIZE)) / 2;
        rect.y += CELLSIZE;
    }
}

void drawPlayer(){
    SDL_Rect rect;
    rect.h = CELLSIZE;
    rect.w = CELLSIZE;
    rect.x = player.x  + (screenDimension.w - (MAPSIZE * CELLSIZE)) / 2;
    rect.y = player.y ;
    SDL_Rect destRect = {32 * (SDL_GetTicks()/200%4), 0, 32, 32};
    SDL_RenderCopyEx(renderer, playerTexture, &destRect, &rect, 0, NULL, SDL_FLIP_NONE);
}

void drawGame(){
    SDL_RenderClear(renderer);
    drawMap();
    drawPlayer();
    SDL_RenderPresent(renderer);
}

void mainLoop(){
    createWindow();

    grassSurface = IMG_Load("Res/grass.png");
    grassTexture = SDL_CreateTextureFromSurface(renderer, grassSurface);

    treeSurface = IMG_Load("Res/tree.png");
    treeTexture = SDL_CreateTextureFromSurface(renderer, treeSurface);

    hoverSurface = IMG_Load("Res/hover.png");
    hoverTexture = SDL_CreateTextureFromSurface(renderer, hoverSurface);

    playerSurface = IMG_Load("Res/character_spritesheet.png");
    playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);

    backgroundSurface = IMG_Load("Res/background.png");
    backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);

    SDL_FreeSurface(grassSurface);
    SDL_FreeSurface(treeSurface);
    SDL_FreeSurface(hoverSurface);
    SDL_FreeSurface(playerSurface);

    unsigned int a = SDL_GetTicks();
    unsigned int b = SDL_GetTicks();
    double delta = 0;

    pthread_t eventThread;
    if (pthread_create(&eventThread, NULL, eventLoop, NULL) != 0){
        printf("Couldn't create thread.");
        exit(EXIT_FAILURE);
    }

    while (running){
        a = SDL_GetTicks();
        delta = (a - b) / 1000.0;
        if (delta > 1/FPS_TO_GET){
            b = a;
            switch (gameState){
                case MENU:
                    //Menu();
                    break;
                case GAME:
                    drawGame();
                    break;
            }
        }
        else {
            // fait dormir le thread pour garder des ressources
            usleep(1000 * (1/FPS_TO_GET - delta));
        }

    }
} 