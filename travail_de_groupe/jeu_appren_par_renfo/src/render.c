#include "render.h"

SDL_Window *window;
SDL_Renderer *renderer;

TTF_Font *RobotoFont;
SDL_DisplayMode screenDimension;

SDL_Rect rect;
SDL_Rect sky;
SDL_Rect ground;

int ** rays;
int  raysListLength = 0;

void initRays(){
    int i;
    rays = malloc(sizeof(int*) * NB_RAYS);
    for (i = 0; i < NB_RAYS; i++){
        rays[i] = malloc(sizeof(int) * 2);
    }
}

void addRayToList(int x, int y){
    if (raysListLength < NB_RAYS){
        *rays[raysListLength] = x;
        *(rays[raysListLength] + 1) = y;
        raysListLength++;
    }
}

void resetRayList(){
    int i;
    for (i = 0; i < NB_RAYS; i++){
        *rays[i] = 0;
        *(rays[i] + 1) = 0;
    }
    raysListLength = 0;
}

// end ray casting variables

void createWindow(){

    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        printf("Couldn't create window.");
        exit(EXIT_FAILURE);
    }

    SDL_GetCurrentDisplayMode(0, &screenDimension);

    window = SDL_CreateWindow("Mat Le King", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenDimension.w, screenDimension.h, SDL_WINDOW_INPUT_GRABBED | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);

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

void endSDL(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    TTF_CloseFont(RobotoFont);
    TTF_Quit();
    SDL_Quit();
}

void drawRays(int map[][MAP_WIDTH]){
    // ray casting variables
    float htexture;
    int r, mx, my, dof;
    double rx, ry, xo, yo, distT;
    double ra;
    resetRayList();
    ra = player.angle - DR * FOV_ANGLE/4;
    if (ra < 0) ra -= 2*pi;
    if (ra > 2*pi) ra -= 2*pi;
    for (r = 0; r<NB_RAYS; r++){
        // check horizontal rays
        dof = 0;
        float disH = 100000, hx = player.x, hy = player.y;
        float aTan = -1/tan(ra);
        if (ra > pi){ // looking up
            ry = (((int)player.y>>6)<<6) - 0.0001;
            rx = (player.y - ry) * aTan + player.x;
            yo = -BLOCK_SIZE;
            xo = -yo*aTan;
        }
        if (ra<pi){ // looking down
            ry = (((int)player.y>>6)<<6) + BLOCK_SIZE;
            rx = (player.y - ry) * aTan + player.x;
            yo = BLOCK_SIZE;
            xo = -yo*aTan;
        }
        if (ra == pi){
            ry = player.y;
            rx = player.x;
            dof = DOF;
        }
        while (dof < DOF){
            mx = (int)rx>>6;
            my = (int)ry>>6;
            if (mx >= 0 && mx < MAP_WIDTH && my >= 0 && my < MAP_HEIGHT && map[my][mx] == 1){
                
                hx = rx;
                hy = ry;
                disH = sqrt((rx-player.x)*(rx-player.x) + (ry-player.y)*(ry-player.y));
                dof = DOF;
            }
            else {
                rx += xo;
                ry += yo;
                dof++;
            }
        }

        // check vertical rays
        dof = 0;
        float disV = 100000, vx = player.x, vy = player.y;
        float nTan = -tan(ra);
        if (ra > pi/2 && ra < 3*pi/2){ // looking left
            rx = (((int)player.x>>6)<<6) - 0.0001;
            ry = player.y + (player.x - rx) * nTan;
            xo = -BLOCK_SIZE;
            yo = -xo*nTan;
        }
        if (ra<pi/2 || ra > 3*pi/2){ // looking right
            rx = (((int)player.x>>6)<<6) + BLOCK_SIZE;
            ry = player.y + (player.x - rx) * nTan;
            xo = BLOCK_SIZE;
            yo = -xo*nTan;
        }
        if (ra == pi || ra == 0){
            ry = player.y;
            rx = player.x;
            dof = DOF;
        }
        while (dof < DOF){
            mx = (int)rx>>6;
            my = (int)ry>>6;
            if (mx >= 0 && mx < MAP_WIDTH && my >= 0 && my < MAP_HEIGHT && map[my][mx] == 1){
                vx = rx;
                vy = ry;
                disV = sqrt((rx-player.x)*(rx-player.x) + (ry-player.y)*(ry-player.y));
                dof = DOF;
            }
            else {
                rx += xo;
                ry += yo;
                dof++;
            }
        }
        if (disH < disV) {
            rx = hx;
            ry = hy;
            distT = disH;
        }
        else {
            rx = vx;
            ry = vy;
            distT = disV;
        }
        ra = ra + ANGLE_INC/2;
        if (ra > 2*pi) ra -= 2*pi;
        if (ra < 0) ra += 2*pi;

        // draw column
        float ca = player.angle - ra;
        if (ca < 0) ca += 2*pi;
        if (ca > 2*pi) ca -= 2*pi;
        distT = distT * cos(ca);
        float lineH = (screenDimension.h/2)/distT;

        rect.x = r;
        rect.y = screenDimension.h/2 - lineH;
        rect.w = 1;
        rect.h = (2 * screenDimension.w * lineH/200);

        if (disH < disV) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        }
        else {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        }
        SDL_RenderFillRect(renderer, &rect);

        // draw the ray in the minimap
        addRayToList(rx, ry);

    }
}

void drawMap2D(int map[][MAP_WIDTH]){
    int i, j;
    rect.w = CELL_SIZE;
    rect.h = CELL_SIZE;
    rect.x = 0;
    rect.y = 0;
    for (i = 0; i < MAP_HEIGHT; i++){
        for (j = 0; j < MAP_WIDTH; j++){
            if (map[i][j] == 1){
                SDL_SetRenderDrawColor(renderer, 5, 255, 255, 255);
                SDL_RenderFillRect(renderer, &rect);
            }
            else {
                if (i == player.x/BLOCK_SIZE && j == player.y/BLOCK_SIZE){
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                    SDL_RenderFillRect(renderer, &rect);
                }
                else {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderFillRect(renderer, &rect);
                }
            }
            rect.x += CELL_SIZE;
        }
        rect.y += CELL_SIZE;
        rect.x = 0;
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    for (i = 0; i < NB_RAYS; i++){
        SDL_RenderDrawLine(renderer, player.x * CELL_SIZE / BLOCK_SIZE , player.y * CELL_SIZE / BLOCK_SIZE, rays[i][0] * CELL_SIZE / BLOCK_SIZE, rays[i][1] * CELL_SIZE / BLOCK_SIZE);
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

void drawGame(){
    SDL_RenderClear(renderer);
    drawRays(map);
    drawMap2D(map);
    SDL_RenderPresent(renderer);
}



void mainLoop(){
    createWindow();
    initRays();

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
        if (delta > 1000/FPS_TO_GET){
            //printf("fps: %f\n", 1000/delta);
            b = a;
            switch (game_state){
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
            usleep(1000 * (1000/FPS_TO_GET - delta));
        }
    }

    endSDL();
} 