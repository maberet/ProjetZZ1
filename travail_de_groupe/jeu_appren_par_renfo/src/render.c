#include "render.h"

float timer = 0;

SDL_Window *window;
SDL_Renderer *renderer;

TTF_Font *RobotoFont;
SDL_DisplayMode screenDimension;

SDL_Rect destRect;
SDL_Rect rect;
SDL_Rect sky;
SDL_Rect ground;
SDL_Rect racket;

int showHub = 1;

SDL_Texture *netTexture;
SDL_Texture *netEdgeLeftTexture;
SDL_Texture *netEdgeRightTexture;
SDL_Texture *crowdTexture;
SDL_Texture *playerTexture;
SDL_Texture *ballTexture;
SDL_Texture *skyTexture;
SDL_Texture *groundTexture;
SDL_Texture *racketTexture;
SDL_Texture *iaScoredTexture;
SDL_Texture *humanScoredTexture;

int **rays;
int raysListLength = 0;

int *ray1;
int *ray2;

rayInfo_t raysListHead;

float fps;

SDL_Texture *loadTexture(char *path)
{
    SDL_Surface *surface = IMG_Load(path);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void addRayInfoToList(rayInfo_t *rayInfoHead, rayInfo_t *rayInfo)
{
    rayInfo->next = rayInfoHead->next;
    rayInfoHead->next = rayInfo;
}

void freeRayInfoList(rayInfo_t *rayInfoHead)
{
    rayInfo_t *rayInfo = rayInfoHead->next;
    while (rayInfo != NULL)
    {
        rayInfo_t *next = rayInfo->next;
        free(rayInfo);
        rayInfo = next;
    }
}

rayInfo_t *allocRayInfo(float ra, float distT, int r, int isTransparent, int direction, float htexture, int rx, int ry)
{
    rayInfo_t *rayInfo = malloc(sizeof(rayInfo_t));
    if (rayInfo == NULL)
    {
        printf("Error: malloc failed\n");
        exit(1);
    }
    rayInfo->ra = ra;
    rayInfo->distT = distT;
    rayInfo->r = r;
    rayInfo->isTransparent = isTransparent;
    rayInfo->direction = direction;
    rayInfo->htexture = htexture;
    rayInfo->rx = rx;
    rayInfo->ry = ry;
    rayInfo->next = NULL;
    return rayInfo;
}

void initRays()
{
    int i;
    rays = malloc(sizeof(int *) * 2 * NB_RAYS);
    for (i = 0; i < NB_RAYS * 2; i++)
    {
        rays[i] = malloc(sizeof(int) * 2);
    }
}

int isRaysListEmpty()
{
    return raysListLength == 0;
}

void addRayToList(int x, int y)
{
    if (raysListLength < 2 * NB_RAYS)
    {
        *rays[raysListLength] = x;
        *(rays[raysListLength] + 1) = y;
        raysListLength++;
    }
}

void resetRayList()
{
    int i;
    for (i = 0; i < 2 * NB_RAYS; i++)
    {
        *rays[i] = 0;
        *(rays[i] + 1) = 0;
    }
    raysListLength = 0;
}

// end ray casting variables

void createWindow()
{

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Couldn't create window.");
        exit(EXIT_FAILURE);
    }

    SDL_GetCurrentDisplayMode(0, &screenDimension);

    window = SDL_CreateWindow("Mat Le Tennisman", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenDimension.w, screenDimension.h, SDL_WINDOW_INPUT_GRABBED | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);

    if (window == NULL)
    {
        printf("Couldn't create window");
        exit(EXIT_FAILURE);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL)
    {
        printf("Couldn't create renderer.");
        exit(EXIT_FAILURE);
    }

    if (TTF_Init() == -1)
    {
        exit(EXIT_FAILURE);
    }

    RobotoFont = TTF_OpenFont("Res/Roboto-Black.ttf", 50);
}

void endSDL()
{
    SDL_DestroyTexture(netTexture);
    SDL_DestroyTexture(netEdgeLeftTexture);
    SDL_DestroyTexture(netEdgeRightTexture);
    SDL_DestroyTexture(crowdTexture);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(ballTexture);
    SDL_DestroyTexture(skyTexture);
    SDL_DestroyTexture(groundTexture);
    SDL_DestroyTexture(racketTexture);

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    TTF_CloseFont(RobotoFont);
    TTF_Quit();
    SDL_Quit();
}

void drawRayColumn(rayInfo_t *rayInfo)
{
    float ca = player.angle - rayInfo->ra;
    if (ca < 0)
        ca += 2 * pi;
    if (ca > 2 * pi)
        ca -= 2 * pi;
    rayInfo->distT = rayInfo->distT * cos(ca);

    float lineH = (screenDimension.h / 2) / rayInfo->distT;
    rect.x = rayInfo->r;
    rect.y = (screenDimension.h / 2 + player.viewAngle) - lineH;
    rect.w = 1;
    rect.h = (2 * screenDimension.w * lineH / 20);

    destRect.x = rayInfo->htexture;
    destRect.y = 0;
    destRect.w = 1;
    destRect.h = 64;

    if (rayInfo->isTransparent)
    {
        rect.h *= 1.2;
        if (map[rayInfo->ry / BLOCK_SIZE][rayInfo->rx / BLOCK_SIZE] == 3)
        {
            SDL_RenderCopy(renderer, netEdgeLeftTexture, &destRect, &rect);
        }

        if (map[rayInfo->ry / BLOCK_SIZE][rayInfo->rx / BLOCK_SIZE] == 4)
        {
            SDL_RenderCopy(renderer, netEdgeRightTexture, &destRect, &rect);
        }

        else
        {
            SDL_RenderCopy(renderer, netTexture, &destRect, &rect);
        }
    }
    else
    {
        destRect.x += +64 * (SDL_GetTicks() / 200 % 4);
        if (rayInfo->direction)
        {
            SDL_RenderCopy(renderer, crowdTexture, &destRect, &rect);
        }
        else
        {
            SDL_RenderCopy(renderer, crowdTexture, &destRect, &rect);
        }
    }
}

void drawVerticalWalls()
{
    rayInfo_t *current = raysListHead.next;
    while (current != NULL)
    {
        if (current->direction && !current->isTransparent)
        {
            drawRayColumn(current);
        }
        current = current->next;
    }
}

void drawVerticalNet()
{
    rayInfo_t *current = raysListHead.next;
    while (current != NULL)
    {
        if (current->direction && current->isTransparent)
        {
            drawRayColumn(current);
        }
        current = current->next;
    }
}

void drawHorizentalWalls()
{
    rayInfo_t *current = raysListHead.next;
    while (current != NULL)
    {
        if (!current->direction)
        {
            drawRayColumn(current);
        }
        current = current->next;
    }
}

void castSingleRay(float angle, float *distanceWall, float *distanceNet, int *returnXWall, int *returnYWall, int *returnXNet, int *returnYNet)
{
    // ray casting variables
    int mx, my, dof;
    double rx, ry, rx2, ry2, xo, yo, distT2;
    double ra;
    mx = 0;
    my = 0;
    raysListHead.next = NULL;
    ra = angle;
    if (ra < 0)
        ra -= 2 * pi;
    if (ra > 2 * pi)
        ra -= 2 * pi;
    // check horizontal rays
    int foundSolidWallH = 0;
    dof = 0;
    float disH, hx = player.x, hy = player.y, hx2 = player.x, hy2 = player.y;
    float aTan = -1 / tan(ra);
    if (ra > pi)
    { // looking up
        ry = (((int)player.y >> 6) << 6) - 0.0001;
        rx = (player.y - ry) * aTan + player.x;
        yo = -BLOCK_SIZE;
        xo = -yo * aTan;
    }
    if (ra < pi)
    { // looking down
        ry = (((int)player.y >> 6) << 6) + BLOCK_SIZE;
        rx = (player.y - ry) * aTan + player.x;
        yo = BLOCK_SIZE;
        xo = -yo * aTan;
    }
    if (ra == pi)
    {
        ry = player.y;
        rx = player.x;
        dof = DOF;
    }
    while (dof < DOF)
    {
        mx = (int)rx >> 6;
        my = (int)ry >> 6;
        if (mx >= 0 && mx < MAP_WIDTH && my >= 0 && my < MAP_HEIGHT)
        {
            if (map[my][mx] == 1)
            {
                hx = rx;
                hy = ry;
                disH = sqrt((rx - player.x) * (rx - player.x) + (ry - player.y) * (ry - player.y));
                dof = DOF;
                foundSolidWallH = 1;
            }
            else
            {
                hx2 = rx;
                hy2 = ry;
                dof++;
                rx += xo;
                ry += yo;
            }
        }
        else
        {
            rx += xo;
            ry += yo;
            dof++;
        }
    }

    // check vertical rays
    dof = 0;
    float disV = 100000, disV2 = 100000, vx = player.x, vy = player.y, vx2, vy2;
    float nTan = -tan(ra);
    if (ra > pi / 2 && ra < 3 * pi / 2)
    { // looking left
        rx = (((int)player.x >> 6) << 6) - 0.0001;
        ry = player.y + (player.x - rx) * nTan;
        xo = -BLOCK_SIZE;
        yo = -xo * nTan;
    }
    if (ra < pi / 2 || ra > 3 * pi / 2)
    { // looking right
        rx = (((int)player.x >> 6) << 6) + BLOCK_SIZE;
        ry = player.y + (player.x - rx) * nTan;
        xo = BLOCK_SIZE;
        yo = -xo * nTan;
    }
    if (ra == pi || ra == 0)
    {
        ry = player.y;
        rx = player.x;
        dof = DOF;
    }
    int foundSolidWallV = 0;
    int foundTransparentWallV = 0;
    while (dof < DOF)
    {
        mx = (int)rx >> 6;
        my = (int)ry >> 6;
        if (mx >= 0 && mx < MAP_WIDTH && my >= 0 && my < MAP_HEIGHT && map[my][mx])
        {
            if (map[my][mx] == 1)
            {
                vx = rx;
                vy = ry;
                disV = sqrt((rx - player.x) * (rx - player.x) + (ry - player.y) * (ry - player.y));
                foundSolidWallV = 1;
                dof = DOF;
            }
            else
            {
                vx2 = rx;
                vy2 = ry;
                disV2 = sqrt((rx - player.x) * (rx - player.x) + (ry - player.y) * (ry - player.y));
                foundTransparentWallV = 1;
                dof++;
                rx += xo;
                ry += yo;
            }
        }
        else
        {
            rx += xo;
            ry += yo;
            dof++;
        }
    }

    if (foundTransparentWallV)
    {
        if (disH < disV2)
        {
            rx = hx2;
            ry = hy2;
            distT2 = disV2;
        }
        else
        {
            rx = vx2;
            ry = vy2;
        }
        if (foundSolidWallV)
        {
            if (disH < disV)
            {
                rx2 = hx;
                ry2 = hy;
                distT2 = disH;
            }
            else
            {
                rx2 = vx;
                ry2 = vy;
                distT2 = disV;
            }
        }
        if (foundSolidWallH)
        {
            if (disH < disV)
            {
                rx2 = hx;
                ry2 = hy;
                distT2 = disH;
            }
            else
            {
                rx2 = vx;
                ry2 = vy;
                distT2 = disV;
            }
        }
    }
    else
    {
        if (disH < disV)
        {
            rx = hx;
            ry = hy;
        }
        else
        {
            rx = vx;
            ry = vy;
        }
    }

    *returnXWall = (int)rx2;
    *returnYWall = (int)ry2;
    *distanceWall = distT2;

    *returnXNet = (int)rx;
    *returnYNet = (int)ry2;
    *distanceNet = (int)distT2;
}

void castRays(int map[][MAP_WIDTH])
{
    // ray casting variables
    float htexture, htexture2;
    int r, mx, my, dof;
    double rx, ry, rx2, ry2, xo, yo, distT, distT2;
    double ra;
    mx = 0;
    my = 0;
    resetRayList();
    freeRayInfoList(&raysListHead);
    raysListHead.next = NULL;
    ra = player.angle - DR * FOV_ANGLE / 4;
    if (ra < 0)
        ra -= 2 * pi;
    if (ra > 2 * pi)
        ra -= 2 * pi;
    for (r = 0; r < NB_RAYS; r++)
    {
        // check horizontal rays
        int foundSolidWallH = 0;
        dof = 0;
        float disH = 100000, disH2 = 100000, hx = player.x, hy = player.y, hx2 = player.x, hy2 = player.y;
        float aTan = -1 / tan(ra);
        if (ra > pi)
        { // looking up
            ry = (((int)player.y >> 6) << 6) - 0.0001;
            rx = (player.y - ry) * aTan + player.x;
            yo = -BLOCK_SIZE;
            xo = -yo * aTan;
        }
        if (ra < pi)
        { // looking down
            ry = (((int)player.y >> 6) << 6) + BLOCK_SIZE;
            rx = (player.y - ry) * aTan + player.x;
            yo = BLOCK_SIZE;
            xo = -yo * aTan;
        }
        if (ra == pi)
        {
            ry = player.y;
            rx = player.x;
            dof = DOF;
        }
        while (dof < DOF)
        {
            mx = (int)rx >> 6;
            my = (int)ry >> 6;
            if (mx >= 0 && mx < MAP_WIDTH && my >= 0 && my < MAP_HEIGHT)
            {
                if (map[my][mx] == 1)
                {
                    hx = rx;
                    hy = ry;
                    disH = sqrt((rx - player.x) * (rx - player.x) + (ry - player.y) * (ry - player.y));
                    dof = DOF;
                    foundSolidWallH = 1;
                }
                else
                {
                    hx2 = rx;
                    hy2 = ry;
                    disH2 = sqrt((rx - player.x) * (rx - player.x) + (ry - player.y) * (ry - player.y));
                    dof++;
                    rx += xo;
                    ry += yo;
                }
            }
            else
            {
                rx += xo;
                ry += yo;
                dof++;
            }
        }

        // check vertical rays
        dof = 0;
        float disV = 100000, disV2 = 100000, vx = player.x, vy = player.y, vx2, vy2;
        float nTan = -tan(ra);
        if (ra > pi / 2 && ra < 3 * pi / 2)
        { // looking left
            rx = (((int)player.x >> 6) << 6) - 0.0001;
            ry = player.y + (player.x - rx) * nTan;
            xo = -BLOCK_SIZE;
            yo = -xo * nTan;
        }
        if (ra < pi / 2 || ra > 3 * pi / 2)
        { // looking right
            rx = (((int)player.x >> 6) << 6) + BLOCK_SIZE;
            ry = player.y + (player.x - rx) * nTan;
            xo = BLOCK_SIZE;
            yo = -xo * nTan;
        }
        if (ra == pi || ra == 0)
        {
            ry = player.y;
            rx = player.x;
            dof = DOF;
        }
        int foundSolidWallV = 0;
        int foundTransparentWallV = 0;
        while (dof < DOF)
        {
            mx = (int)rx >> 6;
            my = (int)ry >> 6;
            if (mx >= 0 && mx < MAP_WIDTH && my >= 0 && my < MAP_HEIGHT && map[my][mx])
            {
                if (map[my][mx] == 1)
                {
                    vx = rx;
                    vy = ry;
                    disV = sqrt((rx - player.x) * (rx - player.x) + (ry - player.y) * (ry - player.y));
                    foundSolidWallV = 1;
                    dof = DOF;
                }
                else
                {
                    vx2 = rx;
                    vy2 = ry;
                    disV2 = sqrt((rx - player.x) * (rx - player.x) + (ry - player.y) * (ry - player.y));
                    foundTransparentWallV = 1;
                    dof++;
                    rx += xo;
                    ry += yo;
                }
            }
            else
            {
                rx += xo;
                ry += yo;
                dof++;
            }
        }

        int direction, direction2;

        if (foundTransparentWallV)
        {
            if (disH < disV2)
            {
                rx = hx2;
                ry = hy2;
                distT = disH2;
                direction = 0;
                htexture = (int)(rx) % BLOCK_SIZE;
            }
            else
            {
                rx = vx2;
                ry = vy2;
                distT = disV2;
                direction = 1;
                htexture = (int)(ry) % BLOCK_SIZE;
            }
            if (foundSolidWallV)
            {
                if (disH < disV)
                {
                    rx2 = hx;
                    ry2 = hy;
                    distT2 = disH;
                    direction2 = 0;
                    htexture2 = (int)(rx2) % BLOCK_SIZE;
                }
                else
                {
                    rx2 = vx;
                    ry2 = vy;
                    distT2 = disV;
                    direction2 = 1;
                    htexture2 = (int)(ry2) % BLOCK_SIZE;
                }
            }
            if (foundSolidWallH)
            {
                if (disH < disV)
                {
                    rx2 = hx;
                    ry2 = hy;
                    distT2 = disH;
                    direction2 = 0;
                    htexture2 = (int)(rx2) % BLOCK_SIZE;
                }
                else
                {
                    rx2 = vx;
                    ry2 = vy;
                    distT2 = disV;
                    direction2 = 1;
                    htexture2 = (int)(ry2) % BLOCK_SIZE;
                }
            }
        }
        else
        {
            if (disH < disV)
            {
                rx = hx;
                ry = hy;
                distT = disH;
                direction = 0;
                htexture = (int)(rx) % BLOCK_SIZE;
            }
            else
            {
                rx = vx;
                ry = vy;
                distT = disV;
                direction = 1;
                htexture = (int)(ry) % BLOCK_SIZE;
            }
        }

        ra = ra + ANGLE_INC / 2;
        if (ra > 2 * pi)
            ra -= 2 * pi;
        if (ra < 0)
            ra += 2 * pi;

        // draw ray
        rayInfo_t *column = allocRayInfo(ra, distT, r, foundTransparentWallV, direction, htexture, rx, ry);
        addRayInfoToList(&raysListHead, column);
        if (foundTransparentWallV)
        {
            if (foundSolidWallV)
            {
                rayInfo_t *column = allocRayInfo(ra, distT2, r, 0, direction2, htexture2, rx2, ry2);
                addRayInfoToList(&raysListHead, column);
            }
            else
            {
                rayInfo_t *column = allocRayInfo(ra, distT2, r, 0, direction, htexture2, rx2, ry2);
                addRayInfoToList(&raysListHead, column);
            }
        }

        addRayToList(rx, ry);
        addRayToList(rx2, ry2);
    }
}

void drawEnnemy()
{
    float ennemyAngle = atan2((ennemy.y + ennemy.w / 2) - (player.y + player.w / 2), (ennemy.x + ennemy.w / 2) - (player.x + player.w / 2));
    float ennemyDistance = sqrt((ennemy.x - player.x) * (ennemy.x - player.x) + (ennemy.y - player.y) * (ennemy.y - player.y)) * BLOCK_SIZE;
    int ennemyWidth = 50;
    int ennemyHeight = 200;
    float angleMin = player.angle - (FOV_ANGLE * DR) / 2;
    float angleMax = player.angle + (FOV_ANGLE * DR) / 2;
    if (angleMin < 0)
    {
        angleMin += 2 * pi;
        angleMax += 2 * pi;
    }
    if (angleMax > 2 * pi)
    {
        angleMax -= 2 * pi;
        angleMin -= 2 * pi;
    }
    if (angleMax > 0 && angleMin > 0 && ennemyAngle < 0)
    {
        ennemyAngle += 2 * pi;
    }

    if (ennemyAngle >= angleMin && ennemyAngle <= angleMax)
    {
        rect.x = screenDimension.w / 2 + (screenDimension.w * tan(ennemyAngle - player.angle)) * sqrt(3) * 0.5;
        rect.w = (ennemyWidth * screenDimension.w) / (ennemyDistance / BLOCK_SIZE);
        rect.h = (ennemyHeight * screenDimension.h) / (ennemyDistance / BLOCK_SIZE);
        rect.y = (screenDimension.h / 2 + player.viewAngle) - rect.h / 5;

        destRect.x = 0;
        destRect.y = 0;
        destRect.w = 64;
        destRect.h = 64;
        float angleSum = ennemyAngle + player.angle;
        if (angleSum > 2 * pi)
            angleSum -= 2 * pi;
        if (angleSum < 0)
            angleSum += 2 * pi;

        if (angleSum > 5 * pi / 3 && angleSum <= pi / 3)
        {
            destRect.x = 2 * destRect.w;
        }
        else if (angleSum > pi / 3 && angleSum <= 2 * pi / 3)
        {
            destRect.x = 3 * destRect.w;
        }
        else if (angleSum > 2 * pi / 3 && angleSum <= 4 * pi / 3)
        {
            destRect.x = 0 * destRect.w;
        }
        else if (angleSum > 4 * pi / 3 && angleSum <= 5 * pi / 3)
        {
            destRect.x = 1 * destRect.w;
        }
        SDL_RenderCopy(renderer, playerTexture, &destRect, &rect);
    }
}

int isAngleInRange(float angle, float min, float max)
{
    return ((angle >= min && angle <= max)) || ((angle >= max && angle <= min));
}

void drawBall()
{
    float ballAngle = atan2(ball.y - player.y, ball.x - player.x);
    float ballDistance = sqrt((ball.x - player.x) * (ball.x - player.x) + (ball.y - player.y) * (ball.y - player.y)) * BLOCK_SIZE;
    float ballDistanceX = ballDistance * cos(ballAngle - player.angle);
    float ballViewAngle = atan2(ball.z * BLOCK_SIZE, ballDistanceX);
    int ballWidth = 25;
    int ballHeight = 25;
    float angleMin = player.angle - (FOV_ANGLE * DR) / 2;
    float angleMax = player.angle + (FOV_ANGLE * DR) / 2;
    if (angleMin < 0)
    {
        angleMin += 2 * pi;
        angleMax += 2 * pi;
    }
    if (angleMax > 2 * pi)
    {
        angleMax -= 2 * pi;
        angleMin -= 2 * pi;
    }
    if (angleMax > 0 && angleMin > 0 && ballAngle < 0)
    {
        ballAngle += 2 * pi;
    }

    if (ballAngle >= angleMin && ballAngle <= angleMax)
    {
        rect.x = screenDimension.w / 2 + (screenDimension.w * tan(ballAngle - player.angle)) * sqrt(3) * 0.5;
        rect.w = (ballWidth * screenDimension.w) / (2 * ballDistance / BLOCK_SIZE);
        rect.h = (ballHeight * screenDimension.h) / (2 * ballDistance / BLOCK_SIZE);
        rect.y = (2.5 * screenDimension.h / 4 + player.viewAngle) - 2 * tan(ballViewAngle) * ballDistance;

        destRect.x = 32 * (SDL_GetTicks() / 150 % 4);
        destRect.y = 0;
        destRect.w = 32;
        destRect.h = 32;

        SDL_RenderCopy(renderer, ballTexture, &destRect, &rect);
    }
}

void drawSkyAndGround()
{

    rect.x = 0;
    rect.y = screenDimension.h / 2 + player.viewAngle;
    rect.h = screenDimension.h - rect.y;
    rect.w = screenDimension.w;

    
    SDL_RenderCopy(renderer, groundTexture, NULL, &rect);

    
    sky.x = 0;
    sky.y = 0;
    sky.w = screenDimension.w;
    sky.h = screenDimension.h / 2 + player.viewAngle;

    destRect.x = 500 + (((player.angle + pi) * RD + player.x / BLOCK_SIZE));
    if (player.angle > pi){
        destRect.x = 500 + (((player.angle - pi) * RD + player.x / BLOCK_SIZE));
    }
    destRect.y =0;
    destRect.w = 100;
    destRect.h = 128;
    
    SDL_RenderCopy(renderer, skyTexture, &destRect, &sky);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

void drawRacket()
{
    destRect.x = 0;
    destRect.y = 0;
    destRect.w = 64;
    destRect.h = 32;
    if (player.isHoldingClick){
        destRect.x = 64 * ((int)(player.hitIntensityTimer/100) % 4);
    }
    SDL_RenderCopy(renderer, racketTexture, &destRect, NULL);
}

void drawMap2D(int map[][MAP_WIDTH])
{
    int i, j;
    rect.w = CELL_SIZE;
    rect.h = CELL_SIZE;
    rect.x = 0;
    rect.y = 0;
    // draw ray
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    for (i = 0; i < raysListLength; i++)
    {
        SDL_RenderDrawLine(renderer, player.x * CELL_SIZE / BLOCK_SIZE, player.y * CELL_SIZE / BLOCK_SIZE, rays[i][0] * CELL_SIZE / BLOCK_SIZE, rays[i][1] * CELL_SIZE / BLOCK_SIZE);
    }
    // draw map
    for (i = 0; i < MAP_HEIGHT; i++)
    {
        for (j = 0; j < MAP_WIDTH; j++)
        {
            switch (map[i][j])
            {
            // bords du terrain
            case 1:
                SDL_SetRenderDrawColor(renderer, 5, 255, 255, 255);
                SDL_RenderFillRect(renderer, &rect);
                break;
            // filet du milieu
            case 2:
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
                SDL_RenderFillRect(renderer, &rect);
                break;

            // extremites du filet gauche et droit
            case 3:
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
                SDL_RenderFillRect(renderer, &rect);
                break;
            case 4:
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
                SDL_RenderFillRect(renderer, &rect);
                break;
            }
            rect.x += CELL_SIZE;
        }
        rect.y += CELL_SIZE;
        rect.x = 0;
    }

    // draw player
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    rect.x = (player.x * CELL_SIZE) / BLOCK_SIZE - CELL_SIZE / 2;
    rect.y = (player.y * CELL_SIZE) / BLOCK_SIZE - CELL_SIZE / 2;
    SDL_RenderFillRect(renderer, &rect);

    // draw ennemi
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    rect.x = (ennemy.x * CELL_SIZE) / BLOCK_SIZE - CELL_SIZE / 2;
    rect.y = (ennemy.y * CELL_SIZE) / BLOCK_SIZE - CELL_SIZE / 2;
    SDL_RenderFillRect(renderer, &rect);

    // draw landing point
    if (landingPointPlayerIsFind == 1)
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        rect.x = landingPointPlayerX * CELL_SIZE;
        rect.y = CELL_SIZE;
        rect.h = (MAP_HEIGHT - 2) * CELL_SIZE;
        rect.w = 3;
        SDL_RenderFillRect(renderer, &rect);
        // reset taille cellule
        rect.h = CELL_SIZE;
        rect.w = CELL_SIZE;
    }
    if (landingPointEnnemyIsFind == 1)
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        rect.x = landingPointEnnemyX * CELL_SIZE;
        rect.y = CELL_SIZE;
        rect.h = (MAP_HEIGHT - 2) * CELL_SIZE;
        rect.w = 3;
        SDL_RenderFillRect(renderer, &rect);
        // reset taille cellule
        rect.h = CELL_SIZE;
        rect.w = CELL_SIZE;
    }

    // draw ball
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    rect.x = (ball.x * CELL_SIZE) / BLOCK_SIZE - CELL_SIZE / 2;
    rect.y = (ball.y * CELL_SIZE) / BLOCK_SIZE - CELL_SIZE / 2;
    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

void drawString(char *str, int x, int y, int w, int h, int r, int g, int b, int a)
{
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

void drawHitIntensity(){
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    rect.w = screenDimension.w/20;
    rect.x = screenDimension.w - rect.w;
    //printf("%f\n", player.hitIntensityTimer);
    rect.h = 2 * screenDimension.h/2 * ( player.hitIntensityTimer/1000);
    rect.y = 3 * screenDimension.h/4 - rect.h;
    SDL_RenderFillRect(renderer, &rect);
}

void drawFPS()
{
    char str[10];
    sprintf(str, "%d", (int)fps);
    drawString(str, screenDimension.w - 50, 0, 50, 50, 255, 255, 255, 255);
}

void drawInfosPlayer()
{
    char str_x_player[10];
    char str_y_player[10];
    char str_z_player[10];
    char str_playerX[20] = "playerX:";
    char str_playerY[20] = "playerY:";
    char str_playerZ[20] = "playerZ:";

    sprintf(str_x_player, "%d", (int)player.x);
    strcat(str_playerX, str_x_player);
    sprintf(str_y_player, "%d", (int)player.y);
    strcat(str_playerY, str_y_player);
    sprintf(str_z_player, "%d", (int)player.z);
    strcat(str_playerZ, str_z_player);

    drawString(str_playerX, screenDimension.w - 120, 50, 100, 50, 255, 255, 255, 255);
    drawString(str_playerY, screenDimension.w - 120, 100, 100, 50, 255, 255, 255, 255);
    drawString(str_playerZ, screenDimension.w - 120, 150, 100, 50, 255, 255, 255, 255);
}

void drawInfosBall()
{
    char str_x_ball[10];
    char str_y_ball[10];
    char str_z_ball[10];
    char str_ballX[20] = "ballX/BS:";
    char str_ballY[20] = "ballY:";
    char str_ballZ[20] = "ballZ:";

    sprintf(str_x_ball, "%d", (int)ball.x / BLOCK_SIZE);
    strcat(str_ballX, str_x_ball);
    sprintf(str_y_ball, "%d", (int)ball.y);
    strcat(str_ballY, str_y_ball);
    sprintf(str_z_ball, "%d", (int)ball.z);
    strcat(str_ballZ, str_z_ball);

    drawString(str_ballX, screenDimension.w - 120, 200, 100, 50, 255, 255, 255, 255);
    drawString(str_ballY, screenDimension.w - 120, 250, 100, 50, 255, 255, 255, 255);
    drawString(str_ballZ, screenDimension.w - 120, 300, 100, 50, 255, 255, 255, 255);
}

void drawHub()
{
    drawInfosPlayer();
    drawInfosBall();
}

void drawWhoScored(){
    switch (whoScored)
    {
        case NONE:
            break;

        case AI:
            SDL_RenderCopy(renderer, iaScoredTexture, NULL, NULL);
            break;

        case PLAYER:
            SDL_RenderCopy(renderer, humanScoredTexture, NULL, NULL);
            break;

        default:
            break;
    }
}

void drawGame()
{
    SDL_RenderClear(renderer);
    drawSkyAndGround();
    castRays(map);
    if (ball.x < MAP_WIDTH * BLOCK_SIZE / 2)
    {
        drawVerticalWalls();
        drawHorizentalWalls();
        drawEnnemy();
        drawVerticalNet();
        drawBall();
    }
    else
    {
        drawVerticalWalls();
        drawHorizentalWalls();
        drawEnnemy();
        // todo bonus : draw point de chute de la balle
        drawBall();
        drawVerticalNet();
    }
    drawMap2D(map);
    drawRacket();
    if (player.isHoldingClick){

        drawHitIntensity();
    }
    drawFPS();
    // affiche le hub
    if (showHub)
    {
        drawHub();
    }
    drawWhoScored();

    SDL_RenderPresent(renderer);
}

void mainLoop()
{
    createWindow();
    initRays();

    netTexture = loadTexture("Res/net.png");
    crowdTexture = loadTexture("Res/crowd.png");
    playerTexture = loadTexture("Res/player_sprite.png");
    ballTexture = loadTexture("Res/ball_sprite.png");
    netEdgeLeftTexture = loadTexture("Res/netLeft.png");
    netEdgeRightTexture = loadTexture("Res/netRight.png");
    skyTexture = loadTexture("Res/sky.png");
    groundTexture = loadTexture("Res/ground.png");
    racketTexture = loadTexture("Res/racket.png");
    iaScoredTexture = loadTexture("Res/iascored.png");
    humanScoredTexture = loadTexture("Res/humanscored.png");

    ray1 = malloc(sizeof(int) * 2);
    ray2 = malloc(sizeof(int) * 2);

    unsigned int a = SDL_GetTicks();
    unsigned int b = SDL_GetTicks();
    double delta = 0;

    pthread_t eventThread;
    if (pthread_create(&eventThread, NULL, EventLoop, NULL) != 0)
    {
        printf("Couldn't create thread.");
        exit(EXIT_FAILURE);
    }

    while (running)
    {
        a = SDL_GetTicks();
        delta = (a - b);
        if (delta > 1000 / FPS_TO_GET)
        {
            fps = 1000 / delta;
            timer += delta;
            b = a;
            switch (game_state)
            {
            case MENU:
                // Menu();
                break;
            case GAME:
                drawGame();
                managePlayer();
                manageEnnemy();
                updateBall();
                break;
            }
        }
        else
        {
            // fait dormir le thread pour garder des ressources
            usleep(1000 * (1000 / FPS_TO_GET - delta));
        }
    }
    endSDL();
}