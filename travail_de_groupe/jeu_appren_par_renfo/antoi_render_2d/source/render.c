#include "render.h"

SDL_Window *window;
SDL_Renderer *renderer;
int window_width = 900;
int window_height = 700;

TTF_Font *robotoFont;

int zoom = 40;
// terrain devrait etre un .c et .h comme canon
//  on aurait un SDL_Rect en terrain TopView et SideView
// là mélange bizarre mais fonctionnel
SDL_Rect terrain;
SDL_Rect drawerTerrainSideView; // drawerTerrain pour les deux vues

SDL_Rect filet;
// drawerFilet

// canon est un .c
SDL_Rect drawerCanon;

// point de chute devrait etre un .c et .h
SDL_Rect point_de_chute;
SDL_Rect drawerPointDeChute;

int zone_chute = -1;

void createWindow()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Couldn't create window.");
        exit(EXIT_FAILURE);
    }

    window = SDL_CreateWindow("Badminton learning", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_RESIZABLE);

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

    robotoFont = TTF_OpenFont("assets/Roboto-Black.ttf", 50);
}

void endSDL()
{
    TTF_CloseFont(robotoFont);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void initTerrain()
{
    terrain.x = 50;
    terrain.y = 50;
    terrain.h = 13.40 * zoom;
    terrain.w = 5.20 * zoom;
}

void initPointDeChute()
{
    point_de_chute.w = 5;
    point_de_chute.h = 5;
    srand(time(NULL));
    point_de_chute.x = (int)rand() % terrain.w;
    point_de_chute.y = (int)rand() % (terrain.h / 2);
}

int getZoneChute(int terrainX, int terrainY, int terrainW, int terrainH)
{
    int z = -1;
    // pdc = point de chute
    int pdc_y = terrainY + terrainH / 2 + point_de_chute.y;
    // en haut à gauche => 1
    if (point_de_chute.x < terrainW / 2 && pdc_y < terrainY + (3 * terrainH) / 4)
    {
        z = 1;
    }
    // en haut à droite => 2
    else if (point_de_chute.x >= terrainW / 2 && pdc_y < terrainY + (3 * terrainH) / 4)
    {
        z = 2;
    }
    // en bas à gauche => 3
    else if (point_de_chute.x < terrainW / 2 && pdc_y >= terrainY + (3 * terrainH) / 4)
    {
        z = 3;
    }
    // en bas à droite => 4
    else
    {
        z = 4;
    }
    return z;
}

void newCanon()
{
    srand(time(NULL));
    canon.x = (int)rand() % terrain.w;
    canon.y = (int)rand() % (terrain.h / 2);
}

void drawString(char *text, int x, int y, int w, int h, int r, int g, int b, int a)
{
    SDL_Color color = {r, g, b, a};
    SDL_Surface *surface = TTF_RenderText_Solid(robotoFont, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void drawTerrainTopView()
{
    // filet
    filet.x = terrain.x;
    filet.h = 4;
    filet.y = terrain.y + terrain.h / 2 - filet.h / 2;
    filet.w = terrain.w;

    // terrain en blanc
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &terrain);

    // filet en vert
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &filet);

    // decoupage des zones
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    // verticale zone haut
    SDL_RenderDrawLine(renderer, terrain.x + terrain.w / 2, terrain.y, terrain.x + terrain.w / 2, terrain.y + terrain.h / 2);
    // horizontale zone haut
    SDL_RenderDrawLine(renderer, terrain.x, terrain.y + terrain.h / 4, terrain.x + terrain.w, terrain.y + terrain.h / 4);

    // verticale bas
    SDL_RenderDrawLine(renderer, terrain.x + terrain.w / 2, terrain.y + terrain.h / 2, terrain.x + terrain.w / 2, terrain.y + terrain.h);
    // horizontale bas
    SDL_RenderDrawLine(renderer, terrain.x, terrain.y + (3 * terrain.h) / 4, terrain.x + terrain.w, terrain.y + (3 * terrain.h) / 4);
}

void drawTerrainSideView()
{
    // terrain
    drawerTerrainSideView.x = terrain.x + terrain.w + 50;
    drawerTerrainSideView.y = terrain.y + terrain.h;
    drawerTerrainSideView.h = 4;
    drawerTerrainSideView.w = 13.40 * zoom;

    // filet
    filet.w = 4;
    filet.x = drawerTerrainSideView.x + drawerTerrainSideView.w / 2 - filet.w / 2;
    filet.h = -1.55 * zoom;
    filet.y = drawerTerrainSideView.y;

    // terrain en blanc
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &drawerTerrainSideView);

    // filet en vert
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &filet);
}

void drawCanonTopView()
{
    // canon
    drawerCanon.w = canon.width;
    drawerCanon.h = canon.length;
    drawerCanon.x = terrain.x + canon.x;
    drawerCanon.y = terrain.y + canon.y;

    // canon en noir
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &drawerCanon);
}

void drawPointDeChuteTopView()
{
    // point de chute de la balle
    drawerPointDeChute.x = terrain.x + point_de_chute.x;
    drawerPointDeChute.y = terrain.y + terrain.h / 2 + point_de_chute.y;
    drawerPointDeChute.w = point_de_chute.w;
    drawerPointDeChute.h = point_de_chute.h;

    // point de chute de la balle
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &drawerPointDeChute);
}

void drawCanonSideView()
{
    // canon
    drawerCanon.w = canon.length;
    drawerCanon.h = canon.height;
    drawerCanon.x = drawerTerrainSideView.x + canon.y;
    drawerCanon.y = drawerTerrainSideView.y - canon.height;

    // canon en noir
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &drawerCanon);
}

void drawPointDeChuteSideView()
{
    // point de chute de la balle
    drawerPointDeChute.x = drawerTerrainSideView.x + drawerTerrainSideView.w / 2 + point_de_chute.y;
    drawerPointDeChute.y = drawerTerrainSideView.y;
    drawerPointDeChute.w = point_de_chute.w;
    drawerPointDeChute.h = point_de_chute.h;

    // point de chute de la balle
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &drawerPointDeChute);
}

void drawInformations()
{
    int texte_width = 200;
    int texte_height = 40;
    char str[20];
    char str2[20];
    char zoneChuteChaine[20] = "zone chute :";
    char zoneCanonChaine[20] = "zone canon :";
    drawString("informations :", window_width - texte_width, texte_height * 0, texte_width, texte_height, 255, 255, 255, 255);
    drawString("e : new point chute", window_width - texte_width, texte_height * 1, texte_width, texte_height, 255, 255, 255, 255);
    sprintf(str2, "%d", zone_chute);
    strcat(zoneChuteChaine, str2);
    drawString(zoneChuteChaine, window_width - texte_width, texte_height * 2, texte_width, texte_height, 255, 255, 255, 255);
    drawString("r : new canon", window_width - texte_width, texte_height * 4, texte_width, texte_height, 255, 255, 255, 255);
    sprintf(str, "%d", canon.zone);
    strcat(zoneCanonChaine, str);
    drawString(zoneCanonChaine, window_width - texte_width, texte_height * 5, texte_width, texte_height, 255, 255, 255, 255);
}

void drawTrajectoireTopView()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderDrawLine(renderer, terrain.x + canon.x, terrain.y + canon.y, drawerPointDeChute.x, drawerPointDeChute.y);
}

void drawTrajectoireSideView()
{
    // crée la liste des points de la trajectoire
    calculTrajectoireAntoine2(
        terrain.x + terrain.w + 50 + canon.y,                                     // x depart
        terrain.y + terrain.h,                                                    // y depart
        terrain.x + terrain.w + 50 + terrain.h / 2,                               // x filet
        terrain.y + terrain.h - 100,                                              // y filet
        drawerTerrainSideView.x + drawerTerrainSideView.w / 2 + point_de_chute.y, // x target
        terrain.y + terrain.h                                                     // y target
    );

    // parcours la liste des points de la trajectoire et l'affiche
    for (int i = 0; i < NUMBERPOINT_TRAJEC - 1; i++)
    {
        SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
        SDL_RenderDrawLine(renderer, trajectoireAntoine[i][0], trajectoireAntoine[i][1], trajectoireAntoine[i + 1][0], trajectoireAntoine[i + 1][1]);
    }
}

void mainLoop()
{
    createWindow();
    initCanon();
    initTerrain();
    initPointDeChute();

    pthread_t eventThread;
    if (pthread_create(&eventThread, NULL, eventLoop, NULL) != 0)
    {
        printf("Couldn't create thread.");
        exit(EXIT_FAILURE);
    }

    while (running)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // top view
        drawTerrainTopView();
        drawCanonTopView();
        drawPointDeChuteTopView();
        drawTrajectoireTopView();
        canon.zone = getZone(terrain.x, terrain.y, terrain.w, terrain.h);
        zone_chute = getZoneChute(terrain.x, terrain.y, terrain.w, terrain.h);

        // side view
        drawTerrainSideView();
        drawCanonSideView();
        drawPointDeChuteSideView();
        drawTrajectoireSideView();

        //
        drawInformations();

        SDL_RenderPresent(renderer);
    }
    endSDL();
}