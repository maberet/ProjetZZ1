#include "render.h"

SDL_Window *window;
SDL_Renderer *renderer;

TTF_Font *RobotoFont;
SDL_DisplayMode screenDimension;

SDL_Rect destRect;
SDL_Rect rect;
SDL_Rect sky;
SDL_Rect ground;

SDL_Texture * netTexture;
SDL_Texture * crowdTexture;
SDL_Texture * playerTexture;

int ** rays;
int  raysListLength = 0;

int * ray1;
int * ray2;

rayInfo_t raysListHead;

float fps;

SDL_Texture * loadTexture(char * path) {
    SDL_Surface * surface = IMG_Load(path);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void addRayInfoToList(rayInfo_t * rayInfoHead, rayInfo_t * rayInfo) {
    rayInfo->next = rayInfoHead->next;
    rayInfoHead->next = rayInfo;
}

void freeRayInfoList(rayInfo_t * rayInfoHead) {
    rayInfo_t * rayInfo = rayInfoHead->next;
    while (rayInfo != NULL) {
        //printf("freeing : %p\n", rayInfo);
        rayInfo_t * next = rayInfo->next;
        free(rayInfo);
        rayInfo = next;
    }
}

rayInfo_t * allocRayInfo(float ra, float distT, int r, int isTransparent, int direction, float htexture){
    rayInfo_t * rayInfo = malloc(sizeof(rayInfo_t));
    if (rayInfo == NULL) {
        printf("Error: malloc failed\n");
        exit(1);
    }
    rayInfo->ra = ra;
    rayInfo->distT = distT;
    rayInfo->r = r;
    rayInfo->isTransparent = isTransparent;
    rayInfo->direction = direction;
    rayInfo->htexture = htexture;
    rayInfo->next = NULL;
    return rayInfo;
}

void initRays(){
    int i;
    rays = malloc(sizeof(int*) * 2 * NB_RAYS);
    for (i = 0; i < NB_RAYS * 2; i++){
        rays[i] = malloc(sizeof(int) * 2);
    }
}

int isRaysListEmpty(){
    return raysListLength == 0;
}

void addRayToList(int x, int y){
    if (raysListLength < 2 * NB_RAYS){
        *rays[raysListLength] = x;
        *(rays[raysListLength] + 1) = y;
        raysListLength++;
    }
}

void resetRayList(){
    int i;
    for (i = 0; i < 2 * NB_RAYS; i++){
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


void drawRayColumn(rayInfo_t * rayInfo){
    float ca = player.angle - rayInfo->ra;
    if (ca < 0) ca += 2*pi;
    if (ca > 2*pi) ca -= 2*pi;
    rayInfo->distT = rayInfo->distT * cos(ca);

    float lineH = (screenDimension.h/2)/rayInfo->distT;
    rect.x = rayInfo->r;
    rect.y = (screenDimension.h/2 + player.viewAngle) - lineH;
    rect.w = 1;
    rect.h = (2 * screenDimension.w * lineH/20);

    destRect.x = rayInfo->htexture;
    destRect.y = 0;
    destRect.w = 1;
    destRect.h = 64;

    if (rayInfo->isTransparent){
        rect.h *= 1.75;
        rect.y -= rect.h/3;
        SDL_RenderCopy(renderer, netTexture, &destRect, &rect);

    }
    else {
        destRect.x += + 64 * (SDL_GetTicks()/200 % 4);
        if (rayInfo->direction){
            SDL_RenderCopy(renderer, crowdTexture, &destRect, &rect);
        }
        else {
            SDL_RenderCopy(renderer, crowdTexture, &destRect, &rect);
        }
    }
}

void drawVerticalRays(){
    rayInfo_t * current = raysListHead.next;
    while (current != NULL){
        //printf("%p\n", current);
        if (current->direction){
            drawRayColumn(current);
        }
        current = current->next;
    }
}

void drawHorizentalRays(){
    rayInfo_t * current = raysListHead.next;
    while (current != NULL){
        //printf("%p\n", current);
        if (!current->direction){
            drawRayColumn(current);
        }
        current = current->next;
    }
}

void castRays(int map[][MAP_WIDTH]){
    // ray casting variables
    float htexture, htexture2;
    int r, mx, my, dof;
    double rx, ry, rx2, ry2,  xo, yo, distT, distT2;
    double ra;
    mx = 0;
    my = 0;
    resetRayList();
    freeRayInfoList(&raysListHead);
    raysListHead.next = NULL;
    ra = player.angle - DR * FOV_ANGLE/4;
    if (ra < 0) ra -= 2*pi;
    if (ra > 2*pi) ra -= 2*pi;
    for (r = 0; r<NB_RAYS; r++){
        // check horizontal rays
        //int foundTransparentWallH = 0;
        int foundSolidWallH = 0;
        dof = 0;
        float disH = 100000, disH2 = 100000, hx = player.x, hy = player.y , hx2 = player.x, hy2 = player.y;
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
            if (mx >= 0 && mx < MAP_WIDTH && my >= 0 && my < MAP_HEIGHT){
                if (map[my][mx] == 1){
                    hx = rx;
                    hy = ry;
                    disH = sqrt((rx-player.x)*(rx-player.x) + (ry-player.y)*(ry-player.y));
                    dof = DOF;
                    foundSolidWallH = 1;
                }
                else {
                    hx2 = rx;
                    hy2 = ry;
                    disH2 = sqrt((rx-player.x)*(rx-player.x) + (ry-player.y)*(ry-player.y));
                    //foundTransparentWallH = 1;
                    dof++;
                    rx += xo;
                    ry += yo;
                }
            }
            else {
                rx += xo;
                ry += yo;
                dof++;
            }
        }

        //printf("hx %f hy %f\n", hx, hy);

        // check vertical rays
        dof = 0;
        float disV = 100000, disV2 = 100000 , vx = player.x, vy = player.y, vx2, vy2;
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
        int foundSolidWallV = 0;
        int foundTransparentWallV = 0;
        while (dof < DOF){
            mx = (int)rx>>6;
            my = (int)ry>>6;
            if (mx >= 0 && mx < MAP_WIDTH && my >= 0 && my < MAP_HEIGHT && map[my][mx]){
                if (map[my][mx] == 1){
                    vx = rx;
                    vy = ry;
                    disV = sqrt((rx-player.x)*(rx-player.x) + (ry-player.y)*(ry-player.y));
                    foundSolidWallV = 1;
                    dof = DOF;
                }
                else {
                    vx2 = rx;
                    vy2 = ry;
                    disV2 = sqrt((rx-player.x)*(rx-player.x) + (ry-player.y)*(ry-player.y));
                    foundTransparentWallV = 1;
                    dof++;
                    rx += xo;
                    ry += yo;
                }
            }
            else {
                rx += xo;
                ry += yo;
                dof++;
            }
        }

        int direction, direction2;
        
        if (foundTransparentWallV){
            if (disH < disV2){
                rx = hx2;
                ry = hy2;
                distT = disH2;
                direction = 0;
                htexture = (int)(rx)%BLOCK_SIZE;
            }
            else {
                rx = vx2;
                ry = vy2;
                distT = disV2;
                direction = 1;
                htexture = (int)(ry)%BLOCK_SIZE;
            }
            if (foundSolidWallV){
                if (disH < disV){
                    rx2 = hx;
                    ry2 = hy;
                    distT2 = disH;
                    direction2 = 0;
                    htexture2 = (int)(rx2)%BLOCK_SIZE;
                }
                else {
                    rx2 = vx;
                    ry2 = vy;
                    distT2 = disV;
                    direction2 = 1;
                    htexture2 = (int)(ry2)%BLOCK_SIZE;
                }
            }
            if (foundSolidWallH){
                if (disH < disV){
                    rx2 = hx;
                    ry2 = hy;
                    distT2 = disH;
                    direction2 = 0;
                    htexture2 = (int)(rx2)%BLOCK_SIZE;
                }
                else {
                    rx2 = vx;
                    ry2 = vy;
                    distT2 = disV;
                    direction2 = 1;
                    htexture2 = (int)(ry2)%BLOCK_SIZE;
                }
            }
        }

        else {
            if (disH < disV) {
                rx = hx;
                ry = hy;
                distT = disH;
                direction = 0;
                htexture = (int)(rx)%BLOCK_SIZE;
            }
            else {
                rx = vx;
                ry = vy;
                distT = disV;
                direction = 1;
                htexture = (int)(ry)%BLOCK_SIZE;
            }
        }


        ra = ra + ANGLE_INC/2;
        if (ra > 2*pi) ra -= 2*pi;
        if (ra < 0) ra += 2*pi;

        // draw ray
        rayInfo_t * column = allocRayInfo(ra, distT, r, foundTransparentWallV, direction, htexture);
        addRayInfoToList(&raysListHead, column);
        if (foundTransparentWallV){
            if (foundSolidWallV){
                rayInfo_t * column = allocRayInfo(ra, distT2, r, 0, direction2  , htexture2);
                addRayInfoToList(&raysListHead, column);
            }
            else {
                rayInfo_t * column = allocRayInfo(ra, distT2, r, 0, direction, htexture2);
                addRayInfoToList(&raysListHead, column);
            }
        }
        // draw the ray in the minimap
        if (r == 0){
            //printf("%d %d\n", (int)rx, (int)ry);
            ray1[0] = (int)rx;
            ray1[1] = (int)ry;
            //printf("ray1 %d %d\n", ray1[0], ray1[1]);
            //printf("ray2 %d %d\n", ray2[0], ray2[1]);
        }
        if (r == NB_RAYS - 1){
            //printf("%d %d\n", (int)rx, (int)ry);
            ray2[0] = (int)rx;
            ray2[1] = (int)ry;
            printf("ray1 %d %d\n", ray1[0]/BLOCK_SIZE, ray1[1]/BLOCK_SIZE);
            printf("ray2 %d %d\n", ray2[0]/BLOCK_SIZE, ray2[1]/BLOCK_SIZE);
        }
        //printf("raylistlength %d\n", raysListLength);
        addRayToList(rx, ry);
        addRayToList(rx2, ry2);

    }
}

void drawEnnemy(){
    float ennemyAngle = atan2((ennemy.y + ennemy.w/2)  - (player.y + player.w/2) , (ennemy.x + ennemy.w/2) - (player.x + player.w/2));
    if (ennemyAngle < 0) ennemyAngle += 2*pi;
    if (ennemyAngle > 2*pi) ennemyAngle -= 2*pi;
    float ennemyDistance = sqrt((ennemy.x - player.x)*(ennemy.x - player.x) + (ennemy.y - player.y)*(ennemy.y - player.y)) * BLOCK_SIZE;
    float ennemyBaseWidth = BLOCK_SIZE;
    float ennemyDistanceX = ennemyDistance * cos(ennemyAngle - player.angle) * BLOCK_SIZE;
    float ennemyDistanceY = ennemyDistance * fabs(sin(ennemyAngle - player.angle)) * BLOCK_SIZE;
    float scaledEnnemyWidth = ennemyBaseWidth / sqrt(3);
    int ennemyWidth = 50;
    int ennemyHeight = 200;

    //printf("%f %f\n", ennemyAngle, player.angle - (FOV_ANGLE)/2 * DR);
    //printf("%f\n", player.angle * RD);

    if (ennemyAngle >= player.angle - (FOV_ANGLE)/2 * DR && ennemyAngle <= player.angle + (FOV_ANGLE)/2 * DR){
        rect.x = screenDimension.w/2 + (screenDimension.w * tan(ennemyAngle - player.angle)) * sqrt(3) * 0.5;
        rect.w = (ennemyWidth * screenDimension.w) / (ennemyDistance/BLOCK_SIZE);
        rect.h = (ennemyHeight * screenDimension.h)/(ennemyDistance/BLOCK_SIZE);
        rect.y = (screenDimension.h/2 + player.viewAngle) - rect.h/5;

        destRect.x = 0;
        destRect.y = 0;
        destRect.w = 64;
        destRect.h = 64;
        //printf("%d %d %d %d\n", rect.x, rect.y, rect.w, rect.h);
        SDL_RenderCopy(renderer, playerTexture, &destRect, &rect);
    }
}

void drawBall(){
    float ballAngle = atan2((ball.y + ball.w/2)  - (player.y + player.w/2) , (ball.x + ball.w/2) - (player.x + player.w/2));
    if (ballAngle < 0) ballAngle += 2*pi;
    if (ballAngle > 2*pi) ballAngle -= 2*pi;
    float ballDistance = sqrt((ball.x - player.x)*(ball.x - player.x) + (ball.y - player.y)*(ball.y - player.y)) * BLOCK_SIZE;
    float ballBaseWidth = BLOCK_SIZE/2;
    float ballDistanceX = ballDistance * cos(ballAngle - player.angle) * BLOCK_SIZE;
    float ballDistanceY = ballDistance * fabs(sin(ballAngle - player.angle)) * BLOCK_SIZE;
    float scaledBallWidth = ballBaseWidth / sqrt(3);
    int ballWidth = 25;
    int ballHeight = 25;

    if (ballAngle >= player.angle - (FOV_ANGLE)/2 * DR && ballAngle <= player.angle + (FOV_ANGLE)/2 * DR){
        rect.x = screenDimension.w/2 + (screenDimension.w * tan(ballAngle - player.angle)) * sqrt(3) * 0.5;
        rect.w = (ballWidth * screenDimension.w) / (ballDistance/BLOCK_SIZE);
        rect.h = (ballHeight * screenDimension.h)/(ballDistance/BLOCK_SIZE);
        rect.y = (screenDimension.h/2 + player.viewAngle) - rect.h/5;

        destRect.x = 0;
        destRect.y = 0;
        destRect.w = 64;
        destRect.h = 64;
        //printf("%d %d %d %d\n", rect.x, rect.y, rect.w, rect.h); 
        SDL_RenderCopy(renderer, playerTexture, &destRect, &rect);
    }
}

void drawSkyAndGround(){
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, NULL);

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    sky.x = 0;
    sky.y = 0;
    sky.w = screenDimension.w;
    sky.h = screenDimension.h/2 + player.viewAngle;
    SDL_RenderFillRect(renderer, &sky);
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

void drawMap2D(int map[][MAP_WIDTH]){
    int i, j;
    rect.w = CELL_SIZE;
    rect.h = CELL_SIZE;
    rect.x = 0;
    rect.y = 0;
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    for (i = 0; i < raysListLength; i++){
        SDL_RenderDrawLine(renderer, player.x * CELL_SIZE / BLOCK_SIZE , player.y * CELL_SIZE / BLOCK_SIZE, rays[i][0] * CELL_SIZE / BLOCK_SIZE, rays[i][1] * CELL_SIZE / BLOCK_SIZE);
    }
    for (i = 0; i < MAP_HEIGHT; i++){
        for (j = 0; j < MAP_WIDTH; j++){
            switch (map[i][j])
            {
            case 1:
                SDL_SetRenderDrawColor(renderer, 5, 255, 255, 255);
                SDL_RenderFillRect(renderer, &rect);
                break;

            case 2:
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
                SDL_RenderFillRect(renderer, &rect);
                break;
            }
            if ((i == player.x/BLOCK_SIZE && j == player.y/BLOCK_SIZE) || (i == ennemy.x/BLOCK_SIZE && j == ennemy.y/BLOCK_SIZE)){
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderFillRect(renderer, &rect);
            }
            rect.x += CELL_SIZE;
        }
        rect.y += CELL_SIZE;
        rect.x = 0;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

void drawString(char *str, int x, int y, int w, int h, int r, int g, int b, int a){
    SDL_Color color = {r, g, b, a};
    SDL_Surface *text = TTF_RenderText_Solid(RobotoFont, str, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, text);
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_FreeSurface(text);
    SDL_DestroyTexture(texture);
}

void drawFPS(){
    char str[10];
    sprintf(str, "%d", (int)fps);
    drawString(str, screenDimension.w - 50, 0, 50, 50, 255, 255, 255, 255);
}

void drawGame(){
    SDL_RenderClear(renderer);
    drawSkyAndGround();
    castRays(map);
    drawHorizentalRays();
    drawEnnemy();
    drawVerticalRays();
    drawBall();
    drawMap2D(map);
    drawFPS();
    SDL_RenderPresent(renderer);
}



void mainLoop(){
    createWindow();
    initRays();

    netTexture = loadTexture("Res/net.png");
    crowdTexture = loadTexture("Res/crowd.png");
    playerTexture = loadTexture("Res/player_sprite.png");

    ray1 = malloc(sizeof(int) * 2);
    ray2 = malloc(sizeof(int) * 2);

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
            fps = 1000/delta;
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