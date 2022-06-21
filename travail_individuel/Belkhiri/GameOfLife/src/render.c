#include "render.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_DisplayMode ScreenDimension;

TTF_Font *RobotoFont;

SDL_Texture * titleTexture = NULL;
SDL_Surface * titleSurface = NULL;

SDL_Texture * playButtonTexture = NULL;
SDL_Surface * playButtonSurface = NULL;

SDL_Texture * columnTexture = NULL;
SDL_Surface * columnSurface = NULL;

SDL_Texture * backgroundTexture = NULL;
SDL_Surface * backgroundSurface = NULL;


void CreateWindow(){

    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        printf("Couldn't create window.");
        exit(EXIT_FAILURE);
    }

    SDL_GetCurrentDisplayMode(0, &ScreenDimension);

    window = SDL_CreateWindow("Game Of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ScreenDimension.w, ScreenDimension.h, SDL_WINDOW_INPUT_GRABBED | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);

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

    RobotoFont = TTF_OpenFont("Res/Roboto-Black.ttf", 50);  
}

void drawBackground(){
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect rect = {0, 0, ScreenDimension.w, ScreenDimension.h};
    SDL_RenderFillRect(renderer, &rect);
}

void drawBackground2(){
    SDL_Rect rect = {0, 0, ScreenDimension.w, ScreenDimension.h};
    SDL_RenderCopy(renderer, backgroundTexture, NULL, &rect);
}

void drawTitle(){
    int titleWidth, titleHeight;
    SDL_QueryTexture(titleTexture, NULL, NULL, &titleWidth, &titleHeight);
    SDL_Rect titleRect = {ScreenDimension.w/2 - titleWidth/2, ScreenDimension.h/2 - titleHeight, titleWidth, titleHeight};
    SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
}

void drawPlayButton(){
    int titleWidth, titleHeight;
    SDL_QueryTexture(titleTexture, NULL, NULL, &titleWidth, &titleHeight);
    int playButtonWidth, playButtonHeight;
    SDL_QueryTexture(playButtonTexture, NULL, NULL, &playButtonWidth, &playButtonHeight);
    playButtonHeight = playButtonHeight / 2;
    playButtonWidth = playButtonWidth / 2;
    SDL_Rect playButtonRect = {ScreenDimension.w/2 - playButtonWidth/2, ScreenDimension.h/2 + titleHeight, playButtonWidth, playButtonHeight};
    SDL_RenderCopy(renderer, playButtonTexture, NULL, &playButtonRect);
}

void drawMAPSIZE(){
    char str[10];
    sprintf(str, "%d", MAPSIZE);
    SDL_Color textColor = {0, 0, 0};
    SDL_Surface * surface = TTF_RenderText_Solid(RobotoFont, str, textColor);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);

    int titleWidth, titleHeight;
    SDL_QueryTexture(texture, NULL, NULL, &titleWidth, &titleHeight);

    SDL_Rect titleRect = {ScreenDimension.w/2 - titleWidth/2, ScreenDimension.h/1.2 - titleHeight, titleWidth, titleHeight};
    SDL_RenderCopy(renderer, texture, NULL, &titleRect);
}

void drawMenu(){
    SDL_RenderClear(renderer);
    drawBackground();
    drawTitle();
    drawPlayButton();
    drawMAPSIZE();
    SDL_RenderPresent(renderer);
}


void drawMap(){
    int x_offset = ScreenDimension.w/2 - MAPSIZE*CELLSIZE/2;
    for(int i = 0; i < MAPSIZE; i++){
        for(int j = 0; j < MAPSIZE; j++){
            if(map[i][j] == 1){
                SDL_Rect rect = {x_offset + j*CELLSIZE, i*CELLSIZE, CELLSIZE, CELLSIZE};
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}

void drawColumns(){
    int columnWidth, columnHeight;
    SDL_QueryTexture(columnTexture, NULL, NULL, &columnWidth, &columnHeight);
    int x_offset = ScreenDimension.w/2 - MAPSIZE*CELLSIZE/2;
    SDL_Rect leftRect = {0, -50, columnWidth, ScreenDimension.h + 100};
    SDL_Rect rightRect = {x_offset + MAPSIZE*CELLSIZE + columnWidth/2, -50, columnWidth, ScreenDimension.h + 100};
    SDL_RenderCopy(renderer, columnTexture, NULL, &leftRect);
    SDL_RenderCopy(renderer, columnTexture, NULL, &rightRect);
}

void drawGame(){
    SDL_RenderClear(renderer);
    drawBackground();
    drawBackground2();
    drawMap();
    drawColumns();
    SDL_RenderPresent(renderer);
}

void drawStable(){
    char str[20] = "Stable state reached";
    SDL_Color textColor = {0, 0, 0};
    SDL_Surface * surface = TTF_RenderText_Solid(RobotoFont, str, textColor);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);

    int titleWidth, titleHeight;
    SDL_QueryTexture(texture, NULL, NULL, &titleWidth, &titleHeight);

    SDL_Rect titleRect = {ScreenDimension.w/2 - titleWidth/2, ScreenDimension.h/2 - titleHeight, titleWidth, titleHeight};
    SDL_RenderCopy(renderer, texture, NULL, &titleRect);
}

void MainLoop(){
    CreateWindow();

    titleSurface = IMG_Load("Res/title.png");
    playButtonSurface = IMG_Load("Res/playButton.png");
    columnSurface = IMG_Load("Res/column.png");
    backgroundSurface = IMG_Load("Res/background.png");

    titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
    playButtonTexture = SDL_CreateTextureFromSurface(renderer, playButtonSurface);
    columnTexture = SDL_CreateTextureFromSurface(renderer, columnSurface);
    backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);

    SDL_FreeSurface(titleSurface);
    SDL_FreeSurface(playButtonSurface);
    SDL_FreeSurface(columnSurface);
    SDL_FreeSurface(backgroundSurface);

    unsigned int a = SDL_GetTicks();
    unsigned int b = SDL_GetTicks();
    double delta = 0;

    pthread_t eventThread;
    if (pthread_create(&eventThread, NULL, EventLoop, NULL) != 0){
        printf("Couldn't create thread.");
        exit(EXIT_FAILURE);
    }

    while (running){
        a = SDL_GetTicks();
        delta = (a - b);
        if (delta > 1/FPS_TO_GET){
            b = a;
            switch (game_state){
                case MENU:
                    drawMenu();
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