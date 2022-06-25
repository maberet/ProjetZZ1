#include "render.h"

float timer = 0;
int fps;

SDL_Window *window;
SDL_Renderer *renderer;

TTF_Font *robotoFont;
SDL_DisplayMode screenDimension;

SDL_Rect buttonRect;
SDL_Rect playAgainButtonRect;
SDL_Rect quitButtonRect;

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

SDL_Surface * backgroundLostSurface;
SDL_Texture * backgroundLostTexture;

SDL_Surface * playButtonSurface;
SDL_Texture * playButtonTexture;

SDL_Surface * playButtonHoverSurface;
SDL_Texture * playButtonHoverTexture;

SDL_Surface * playAgainButtonSurface;
SDL_Texture * playAgainButtonTexture;

SDL_Surface * playAgainButtonHoverSurface;
SDL_Texture * playAgainButtonHoverTexture;

SDL_Surface * quitButtonSurface;
SDL_Texture * quitButtonTexture;

SDL_Surface * quitButtonHoverSurface;
SDL_Texture * quitButtonHoverTexture;

SDL_Surface * fireSurface;
SDL_Texture * fireTexture;

SDL_Surface * waterSurface;
SDL_Texture * waterTexture;

SDL_Surface * emptyBucketSurface;
SDL_Texture * emptyBucketTexture;

SDL_Surface * filledBucketSurface;
SDL_Texture * filledBucketTexture;

SDL_Surface * heartSurface;
SDL_Texture * heartTexture;

SDL_Surface * scoreSurface;
SDL_Texture * scoreTexture;

SDL_Surface * nextFireSurface;
SDL_Texture * nextFireTexture;

SDL_Surface * backgroundWonSurface;
SDL_Texture * backgroundWonTexture;

void createWindow(){

    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        printf("Couldn't create window.");
        exit(EXIT_FAILURE);
    }

    SDL_GetCurrentDisplayMode(0, &screenDimension);

    window = SDL_CreateWindow("Mat the Firefighter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenDimension.w, screenDimension.h, SDL_WINDOW_INPUT_GRABBED | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);

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

void endSDL(){
    SDL_DestroyTexture(grassTexture);
    SDL_DestroyTexture(treeTexture);
    SDL_DestroyTexture(hoverTexture);
    SDL_DestroyTexture(noHoverTexture);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(playerIdleTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(backgroundSidesTexture);
    SDL_DestroyTexture(backgroundLostTexture);
    SDL_DestroyTexture(playButtonTexture);
    SDL_DestroyTexture(playButtonHoverTexture);
    SDL_DestroyTexture(playAgainButtonTexture);
    SDL_DestroyTexture(playAgainButtonHoverTexture);
    SDL_DestroyTexture(quitButtonTexture);
    SDL_DestroyTexture(quitButtonHoverTexture);
    SDL_DestroyTexture(fireTexture);
    SDL_DestroyTexture(waterTexture);
    SDL_DestroyTexture(emptyBucketTexture);
    SDL_DestroyTexture(filledBucketTexture);
    SDL_DestroyTexture(heartTexture);
    SDL_DestroyTexture(scoreTexture);
    SDL_DestroyTexture(nextFireTexture);
    SDL_DestroyTexture(backgroundWonTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(robotoFont);
    TTF_Quit();
    SDL_Quit();
    if (gameState != WON){
        fireList = freeListFire(fireList);
    }
}

void drawString(char *text, int x, int y, int w, int h, int r, int g, int b, int a){
    SDL_Color color = {r, g, b, a};
    SDL_Surface *surface = TTF_RenderText_Solid(robotoFont, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
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
    int tick = player.invisible * (SDL_GetTicks()/200)%2;
    SDL_SetTextureColorMod(playerIdleTexture, 255 * (1 - tick), 255 * (1-tick), 255 * (1-tick));
    SDL_SetTextureColorMod(playerTexture, 255 * (1 - tick), 255 * (1-tick), 255 * (1-tick));
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
    int loop;
    int step;
    listchainfire_t cour = fireList;
    SDL_Rect rect;
    rect.h = CELLSIZE;
    rect.w = CELLSIZE;
    SDL_Rect srcRect;
    while (cour != NULL){
        rect.x = (cour->fire).x * CELLSIZE + (screenDimension.w - (MAPSIZE * CELLSIZE)) / 2;
        rect.y = (cour->fire).y * CELLSIZE - rect.h/2;

        switch ((cour->fire).state)
        {
            case SPARKLE:
                step = 0;
                loop = 2;
                break;

            case MEDIUM:
                step = 2 * srcRect.w;
                loop = 4;
                break;

            case STRONG:
                step = 6 * srcRect.w;
                loop = 6;
                break;

            case SPREAD:
                step = 6 * srcRect.w;
                loop = 6;
                break;
            
            default:
                break;
        }
        
        srcRect.w = 15;
        srcRect.h = 24;
        srcRect.x = step + srcRect.w * (SDL_GetTicks()/200 % loop);
        srcRect.y= 0;


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

void drawPlayerHP(){
    int borderWidth = (screenDimension.w - (MAPSIZE * CELLSIZE))/2;
    SDL_Rect rect;
    rect.h = borderWidth/player.HPMax;
    rect.w = rect.h;
    SDL_Rect destRect;
    destRect.w = 1024;
    destRect.h = 1024;
    destRect.x = destRect.w * (SDL_GetTicks()/200 % 4);
    destRect.y = 0;
    int count = player.currentHP;
    for (int i=0; i<player.HPMax; i++){
        rect.x = (i*rect.h);
        rect.y = screenDimension.h - 3 * rect.h;
        if (count){
            count--;
            SDL_RenderCopy(renderer, heartTexture, &destRect, &rect);
        }
        else {
            SDL_SetTextureColorMod( heartTexture,0,0,0);
            SDL_RenderCopy(renderer, heartTexture, &destRect, &rect);
            SDL_SetTextureColorMod( heartTexture,255,255,255);

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
    sprintf(str, "%d", player.score);
    drawString(str, rect.x, rect.y, (screenDimension.w - (MAPSIZE * CELLSIZE)) / 2, screenDimension.h/6,237,222,17,255);
}

void drawTime(){
    SDL_Rect rect;
    rect.h = screenDimension.h/6;
    rect.w = (screenDimension.w - (MAPSIZE * CELLSIZE)) / 2;
    rect.x = rect.w + (MAPSIZE * CELLSIZE);
    rect.y = 0;
    SDL_RenderCopy(renderer, nextFireTexture, NULL, &rect);
    rect.y += rect.h;
    char str[10];
    //printf("%d\n", (UPDATETIME * 1000 - (int)timer % (UPDATETIME * 1000))/1000);
    sprintf(str, "%d", (UPDATETIME * 1000 - (int)timer % (UPDATETIME * 1000))/1000);
    drawString(str, rect.x, rect.y, (screenDimension.w - (MAPSIZE * CELLSIZE)) / 2, screenDimension.h/6,237,222,17,255);   
}

void drawFPS(){
    SDL_Rect rect;
    int h = screenDimension.h/12;
    int w = screenDimension.w/12;
    int x = (screenDimension.w + (MAPSIZE * CELLSIZE)) / 2;
    int y = screenDimension.h - rect.h;

    char str[10];
    sprintf(str, "%d", fps);
    drawString(str, x, y, w, h,237,222,17,255);
}

void drawGame(){
    SDL_RenderClear(renderer);
    drawBackgroundSides();
    drawMap();
    drawPlayer();
    drawFire();
    drawPlayerWaterLevel();
    drawPlayerHP();
    drawScore();
    drawTime();
    drawFPS();
    SDL_RenderPresent(renderer);
}


void drawLost(){
    SDL_Rect rect;
    rect.h = screenDimension.h;
    rect.w = screenDimension.w;
    rect.x = 0;
    rect.y = 0;
    SDL_RenderCopy(renderer, backgroundLostTexture, NULL, &rect);
    SDL_RenderCopy(renderer, playAgainButtonHoverTexture, NULL, &rect);
    SDL_RenderCopy(renderer, quitButtonHoverTexture, NULL, &rect);
    SDL_RenderCopy(renderer, playAgainButtonHoverTexture, NULL, &rect);

    // draw score
    rect.h = screenDimension.h/12;
    rect.w = rect.h;
    rect.x = screenDimension.w/2;
    rect.y = screenDimension.h/2 - rect.h;
    char str[10];
    sprintf(str, "%d", player.score);
    drawString(str, rect.x, rect.y, rect.w, rect.h,237,222,17,255);
}

void drawWon(){
    SDL_Rect rect;
    rect.h = screenDimension.h;
    rect.w = screenDimension.w;
    rect.x = 0;
    rect.y = 0;
    SDL_RenderCopy(renderer, backgroundWonTexture, NULL, &rect);
    SDL_RenderCopy(renderer, playAgainButtonHoverTexture, NULL, &rect);
    SDL_RenderCopy(renderer, quitButtonHoverTexture, NULL, &rect);
    SDL_RenderCopy(renderer, playAgainButtonHoverTexture, NULL, &rect);

    // draw score
    rect.h = screenDimension.h/12;
    rect.w = rect.h;
    rect.x = screenDimension.w/2;
    rect.y = screenDimension.h/2 - rect.h;
    char str[10];
    sprintf(str, "%d", player.score);
    drawString(str, rect.x, rect.y, rect.w, rect.h,237,222,17,255);
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

    backgroundLostSurface = IMG_Load("Res/lostScreen.png");
    backgroundLostTexture = SDL_CreateTextureFromSurface(renderer, backgroundLostSurface);

    playButtonSurface = IMG_Load("Res/play_button.png");
    playButtonTexture = SDL_CreateTextureFromSurface(renderer, playButtonSurface);

    playButtonHoverSurface = IMG_Load("Res/play_button_hover.png");
    playButtonHoverTexture = SDL_CreateTextureFromSurface(renderer, playButtonHoverSurface);

    quitButtonSurface = IMG_Load("Res/buttonQuit.png");
    quitButtonTexture = SDL_CreateTextureFromSurface(renderer, quitButtonSurface);

    quitButtonHoverSurface = IMG_Load("Res/buttonQuitHover.png");
    quitButtonHoverTexture = SDL_CreateTextureFromSurface(renderer, quitButtonHoverSurface);

    playAgainButtonSurface = IMG_Load("Res/buttonPlayAgain.png");
    playAgainButtonTexture = SDL_CreateTextureFromSurface(renderer, playAgainButtonSurface);

    playAgainButtonHoverSurface = IMG_Load("Res/buttonPlayAgainHover.png");
    playAgainButtonHoverTexture = SDL_CreateTextureFromSurface(renderer, playAgainButtonHoverSurface);

    fireSurface = IMG_Load("Res/fire_spritesheet.png");
    fireTexture = SDL_CreateTextureFromSurface(renderer, fireSurface);

    waterSurface = IMG_Load("Res/water.png");
    waterTexture = SDL_CreateTextureFromSurface(renderer, waterSurface);

    emptyBucketSurface = IMG_Load("Res/empty_bucket.png");
    emptyBucketTexture = SDL_CreateTextureFromSurface(renderer, emptyBucketSurface);

    filledBucketSurface = IMG_Load("Res/filled_bucket.png");
    filledBucketTexture = SDL_CreateTextureFromSurface(renderer, filledBucketSurface);
    
    scoreSurface = IMG_Load("Res/score.png");
    scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);

    nextFireSurface = IMG_Load("Res/nextFire.png");
    nextFireTexture = SDL_CreateTextureFromSurface(renderer, nextFireSurface);

    heartSurface = IMG_Load("Res/heart_spritesheet.png");
    heartTexture = SDL_CreateTextureFromSurface(renderer, heartSurface);
    
    backgroundWonSurface = IMG_Load("Res/wonScreen.png");
    backgroundWonTexture = SDL_CreateTextureFromSurface(renderer, backgroundWonSurface);

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
    SDL_FreeSurface(backgroundLostSurface);
    SDL_FreeSurface(playAgainButtonSurface);
    SDL_FreeSurface(quitButtonSurface);
    SDL_FreeSurface(quitButtonHoverSurface);
    SDL_FreeSurface(playAgainButtonHoverSurface);
    SDL_FreeSurface(heartSurface);
    SDL_FreeSurface(backgroundWonSurface);

    playAgainButtonRect.x = (screenDimension.w * 700)/1920;
    playAgainButtonRect.y = (screenDimension.h * 615)/1080;
    playAgainButtonRect.w = (screenDimension.w * 400)/1920;
    playAgainButtonRect.h = (screenDimension.h * 130)/1080;

    quitButtonRect.x = playAgainButtonRect.x;
    quitButtonRect.w = playAgainButtonRect.w;
    quitButtonRect.y = (screenDimension.h * 887)/1080;
    quitButtonRect.h = playAgainButtonRect.h;

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
            fps = 1000/delta;
            timer += delta;
            //printf("%d\n", (int)timer % 1000);
            b = a;
            switch (gameState){
                case MENU:
                    drawMenu();
                    break;
                case GAME:
                    if ((int)timer % (UPDATETIME * 1000) < 10){
                        nextFire(fireList);
                        fireList=spreadFire(fireList);
                    }
                    //updateMap();
                    drawGame();
                    if (fireList == NULL){
                        gameState = WON;
                    }
                    break;

                case LOSE:
                    drawLost();
                    break;

                case WON:
                    drawWon();
                    break;
            }
        }
        else {
            // fait dormir le thread pour garder des ressources
            usleep(1000 * (1000/FPS_TO_GET - delta));
        }
    }
    endSDL();
} 