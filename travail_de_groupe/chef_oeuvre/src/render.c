#include "render.h"

int score = 100;
float timer = 0;

SDL_Window *window;
SDL_Renderer *renderer;

TTF_Font *robotoFont;
SDL_DisplayMode screenDimension;

SDL_Rect buttonRect;

SDL_Surface * grassSurface;
SDL_Texture * grassTexture;

SDL_Surface * treeSurface;
SDL_Texture * treeTexture;

SDL_Surface * hoverSurface;
SDL_Texture * hoverTexture;

SDL_Surface * noHoverSurface;
SDL_Texture * noHoverTexture;

SDL_Surface * playerSurface;
SDL_Texture * playerTexture;

SDL_Surface * playerIdleSurface;
SDL_Texture * playerIdleTexture;

SDL_Surface * backgroundSurface;
SDL_Texture * backgroundTexture;

SDL_Surface * backgroundSidesSurface;
SDL_Texture * backgroundSidesTexture;

SDL_Surface * playButtonSurface;
SDL_Texture * playButtonTexture;

SDL_Surface * playButtonHoverSurface;
SDL_Texture * playButtonHoverTexture;

SDL_Surface * fireSurface;
SDL_Texture * fireTexture;

SDL_Surface * waterSurface;
SDL_Texture * waterTexture;

SDL_Surface * emptyBucketSurface;
SDL_Texture * emptyBucketTexture;

SDL_Surface * filledBucketSurface;
SDL_Texture * filledBucketTexture;

SDL_Surface * scoreSurface;
SDL_Texture * scoreTexture;

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

void drawBackground(){
    SDL_Rect rect = {0, 0, screenDimension.w, screenDimension.h};
    SDL_RenderCopy(renderer, backgroundTexture, NULL, &rect);
}

void drawPlayButton(){
    int buttonPosX = screenDimension.w/2 - screenDimension.w/12;
    int buttonPosY = screenDimension.h/2;
    int buttonW = screenDimension.w/6;
    int buttonH = screenDimension.h/6;
    buttonRect.x = buttonPosX;
    buttonRect.y = buttonPosY;
    buttonRect.w = buttonW;
    buttonRect.h = buttonH;

    if (mousePosition.x <= buttonPosX + buttonW && mousePosition.x >= buttonPosX && mousePosition.y >= buttonPosY && mousePosition.y <= buttonPosY + buttonH){
        SDL_RenderCopy(renderer, playButtonHoverTexture, NULL, &buttonRect);
    }
    else {
        SDL_RenderCopy(renderer, playButtonTexture, NULL, &buttonRect);
    }
}

void drawMenu(){
    SDL_RenderClear(renderer);
    drawBackground();
    drawPlayButton();
    SDL_RenderPresent(renderer);
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
            switch (map[i][j]){
                case 1:
                    SDL_RenderCopy(renderer, treeTexture, NULL, &rect);
                    break;
                case 2:
                    SDL_RenderCopy(renderer, waterTexture, NULL, &rect);
                    break;
            }
            if (mousePosition.x == j && mousePosition.y == i){
                if (selectStateHover()){
                    SDL_RenderCopy(renderer, hoverTexture, NULL, &rect);
                }
                else {
                    SDL_RenderCopy(renderer, noHoverTexture, NULL, &rect);
                }
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
    SDL_Rect destRect = {32 * (SDL_GetTicks()/200%6), 0, 32, 32};
    int flip = (player.direction == PLAYER_LEFT) ? (SDL_FLIP_HORIZONTAL) : (SDL_FLIP_NONE);
    if (player.isMoving){
        SDL_RenderCopyEx(renderer, playerTexture, &destRect, &rect, 0, NULL, flip);
    }
    else {
        SDL_RenderCopyEx(renderer, playerIdleTexture, &destRect, &rect, 0, NULL, flip);
    }
}

void drawBackgroundSides(){
    SDL_Rect rect = {0, 0, screenDimension.w, screenDimension.h};
    SDL_RenderCopy(renderer, backgroundSidesTexture, NULL, &rect);
}

void drawFire(){
    listchainfire_t cour = fireList;
    SDL_Rect rect;
    SDL_Rect srcRect;
    srcRect.w = 24;
    srcRect.h =32;
    srcRect.x = srcRect.w * (SDL_GetTicks()/200 % 8);
    srcRect.y= 0;
    while (cour != NULL){
        rect.h = CELLSIZE * (cour->fire).state;
        rect.w = CELLSIZE * (cour->fire).state;
        rect.x = (cour->fire).x * CELLSIZE + (screenDimension.w - (MAPSIZE * CELLSIZE)) / 2 - rect.w/2;
        rect.y = (cour->fire).y * CELLSIZE;
        SDL_RenderCopy(renderer, fireTexture, &srcRect, &rect);
        cour = cour->next;
    }
}

void drawPlayerWaterLevel(){
    int borderWidth = (screenDimension.w - (MAPSIZE * CELLSIZE)) / 2;
    SDL_Rect rect;
    rect.h = borderWidth/player.waterMax;
    rect.w = borderWidth/player.waterMax;
    int count = player.currentWater;
    for (int i=0; i<player.waterMax; i++){
        rect.x = (i*rect.h);
        rect.y = screenDimension.h - 1.5 * rect.h;
        if (count){
            count--;
            SDL_RenderCopy(renderer, filledBucketTexture, NULL, &rect);
        }
        else {
            SDL_RenderCopy(renderer, emptyBucketTexture, NULL, &rect);
        }
    }
}

void drawScore(){
    SDL_Rect rect;
    rect.h = screenDimension.h/6;
    rect.w = (screenDimension.w - (MAPSIZE * CELLSIZE)) / 2;
    rect.x = 0;
    rect.y = 0;
    SDL_RenderCopy(renderer, scoreTexture, NULL, &rect);
    rect.y += rect.h;
    char str[10];
    sprintf(str, "%d", score);
    SDL_Color textColor = {237,222,17};
    SDL_Surface * surface = TTF_RenderText_Solid(robotoFont, str, textColor);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}

void drawGame(){
    SDL_RenderClear(renderer);
    drawBackgroundSides();
    drawMap();
    drawPlayer();
    drawFire();
    drawPlayerWaterLevel();
    drawScore();
    SDL_RenderPresent(renderer);
}

void mainLoop(){
    createWindow();
    initPlayer();

    grassSurface = IMG_Load("Res/grass.png");
    grassTexture = SDL_CreateTextureFromSurface(renderer, grassSurface);

    treeSurface = IMG_Load("Res/tree.png");
    treeTexture = SDL_CreateTextureFromSurface(renderer, treeSurface);

    hoverSurface = IMG_Load("Res/hover.png");
    hoverTexture = SDL_CreateTextureFromSurface(renderer, hoverSurface);

    noHoverSurface = IMG_Load("Res/noHover.png");
    noHoverTexture = SDL_CreateTextureFromSurface(renderer, noHoverSurface);

    playerSurface = IMG_Load("Res/character_tileset.png");
    playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);

    playerIdleSurface = IMG_Load("Res/character_idle_spritesheet.png");
    playerIdleTexture = SDL_CreateTextureFromSurface(renderer, playerIdleSurface);

    backgroundSurface = IMG_Load("Res/background_mat.png");
    backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);

    backgroundSidesSurface = IMG_Load("Res/background_sides.png");
    backgroundSidesTexture = SDL_CreateTextureFromSurface(renderer, backgroundSidesSurface);

    playButtonSurface = IMG_Load("Res/play_button.png");
    playButtonTexture = SDL_CreateTextureFromSurface(renderer, playButtonSurface);

    playButtonHoverSurface = IMG_Load("Res/play_button_hover.png");
    playButtonHoverTexture = SDL_CreateTextureFromSurface(renderer, playButtonHoverSurface);

    fireSurface = IMG_Load("Res/fire.png");
    fireTexture = SDL_CreateTextureFromSurface(renderer, fireSurface);

    waterSurface = IMG_Load("Res/water.png");
    waterTexture = SDL_CreateTextureFromSurface(renderer, waterSurface);

    emptyBucketSurface = IMG_Load("Res/empty_bucket.png");
    emptyBucketTexture = SDL_CreateTextureFromSurface(renderer, emptyBucketSurface);

    filledBucketSurface = IMG_Load("Res/filled_bucket.png");
    filledBucketTexture = SDL_CreateTextureFromSurface(renderer, filledBucketSurface);
    
    scoreSurface = IMG_Load("Res/score.png");
    scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);

    SDL_FreeSurface(grassSurface);
    SDL_FreeSurface(treeSurface);
    SDL_FreeSurface(hoverSurface);
    SDL_FreeSurface(playerSurface);
    SDL_FreeSurface(backgroundSurface);
    SDL_FreeSurface(playButtonSurface);
    SDL_FreeSurface(playButtonHoverSurface);
    SDL_FreeSurface(backgroundSidesSurface);
    SDL_FreeSurface(noHoverSurface);
    SDL_FreeSurface(fireSurface);
    SDL_FreeSurface(waterSurface);
    SDL_FreeSurface(emptyBucketSurface);
    SDL_FreeSurface(filledBucketSurface);
    SDL_FreeSurface(scoreSurface);


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
        delta = (a - b);
        if (delta > 1000/FPS_TO_GET){
            timer += delta;
            printf("timer : %f\n", timer/1000);
            b = a;
            printf("fps : %f", 1000/delta);
            switch (gameState){
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
            usleep(1000 * (1000/FPS_TO_GET - delta));
        }

    }
} 