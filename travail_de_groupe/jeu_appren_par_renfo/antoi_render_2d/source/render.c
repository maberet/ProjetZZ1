#include "render.h"

SDL_Window *window;
SDL_Renderer *renderer;
int window_width = 900;
int window_height = 700;

TTF_Font *robotoFont;

int zoom = 40;
//terrain devrait etre un .c et .h comme canon
// on aurait un SDL_Rect en terrain TopView et SideView
//là mélange bizarre mais fonctionnel
SDL_Rect terrain;
SDL_Rect drawerTerrainSideView;

SDL_Rect filet;
SDL_Rect canon_rect;
SDL_Rect point_de_chute;

int point_x_rand;
int point_y_rand;

int zone_canon = -1;
int zone_chute = -1;

void createWindow(){
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        printf("Couldn't create window.");
        exit(EXIT_FAILURE);
    }

    window = SDL_CreateWindow("Badminton learning", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_RESIZABLE);

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

    robotoFont = TTF_OpenFont("assets/Roboto-Black.ttf", 50);  
}

void endSDL(){
    TTF_CloseFont(robotoFont);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void initTerrain(){
    terrain.x = 50;
    terrain.y = 50; 
    terrain.h = 13.40 * zoom;
    terrain.w = 5.20 * zoom;
}

void initPointDeChute(){
    srand(time(NULL));
    point_x_rand = (int)rand()%terrain.w;
    point_y_rand = (int)rand()%(terrain.h/2);
}

int getZoneChute(int terrainX, int terrainY, int terrainW, int terrainH){
    int z = -1;
    // pdc = point de chute
    int pdc_y = terrainY + terrainH/2 + point_y_rand;
    //en haut à gauche => 1
    if(point_x_rand>=0 && point_x_rand<terrainW/2 && pdc_y<terrainY+(3*terrainH)/4){
        z = 1;
    }
    //en haut à droite => 2
    else if(point_x_rand>=terrainW/2 && point_x_rand<=terrainW && pdc_y<terrainY+(3*terrainH)/4){
        z = 2;
    }
    //en bas à gauche => 3
    else if(point_x_rand>=0 && point_x_rand<terrainW/2 && pdc_y>=terrainY+(3*terrainH)/4){
        z = 3;
    }
    //en bas à droite => 4
    else{
        z = 4;
    }
    return z;
}

void newCanon(){
    srand(time(NULL));
    canon.x = (int)rand()%terrain.w;
    canon.y = (int)rand()%(terrain.h/2);
}

void drawString(char *text, int x, int y, int w, int h, int r, int g, int b, int a){
    SDL_Color color = {r, g, b, a};
    SDL_Surface *surface = TTF_RenderText_Solid(robotoFont, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void drawTerrainTopView(){
    //filet
    filet.x = terrain.x;
    filet.h = 4;
    filet.y = terrain.y + terrain.h/2 - filet.h/2; 
    filet.w = terrain.w;
    
    //terrain en blanc
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &terrain);

    //filet en vert
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &filet);

    //decoupage des zones
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    //verticale zone haut
    SDL_RenderDrawLine(renderer, terrain.x+terrain.w/2, terrain.y, terrain.x+terrain.w/2, terrain.y+terrain.h/2);
    //horizontale zone haut
    SDL_RenderDrawLine(renderer, terrain.x, terrain.y+terrain.h/4, terrain.x+terrain.w, terrain.y+terrain.h/4);

    //verticale bas
    SDL_RenderDrawLine(renderer, terrain.x+terrain.w/2, terrain.y+terrain.h/2, terrain.x+terrain.w/2, terrain.y+terrain.h);
    //horizontale bas
    SDL_RenderDrawLine(renderer, terrain.x, terrain.y+(3*terrain.h)/4, terrain.x+terrain.w, terrain.y+(3*terrain.h)/4);

}

void drawTerrainSideView(){
    //terrain
    drawerTerrainSideView.x = terrain.x + terrain.w + 50;
    drawerTerrainSideView.y = terrain.y + terrain.h; 
    drawerTerrainSideView.h = 4;
    drawerTerrainSideView.w = 13.40 * zoom;

    //filet
    filet.w = 4;
    filet.x = drawerTerrainSideView.x + drawerTerrainSideView.w/2 - filet.w/2;
    filet.h = -1.55 * zoom;
    filet.y = drawerTerrainSideView.y; 
    
    //terrain en blanc
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &drawerTerrainSideView);

    //filet en vert
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &filet);
}

void drawCanonTopView(){
    //canon 
    canon_rect.w = canon.width; 
    canon_rect.h = canon.length;
    canon_rect.x = terrain.x + canon.x;
    canon_rect.y = terrain.y + canon.y;

    //canon en noir
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &canon_rect);
}

void drawPointDeChuteTopView(){
    //point de chute de la balle
    point_de_chute.w = 5;
    point_de_chute.h = 5;
    point_de_chute.x = (int)terrain.x + point_x_rand;
    point_de_chute.y = (int)terrain.y + terrain.h/2 + point_y_rand;

    //point de chute de la balle
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &point_de_chute);
}

void drawCanonSideView(){
    //canon 
    canon_rect.w = canon.length; 
    canon_rect.h = canon.height;
    canon_rect.x = drawerTerrainSideView.x + canon.y;
    canon_rect.y = drawerTerrainSideView.y - canon.height;

    //canon en noir
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &canon_rect);
}

void drawPointDeChuteSideView(){
    //point de chute de la balle
    point_de_chute.w = 5;
    point_de_chute.h = 5;
    point_de_chute.x = drawerTerrainSideView.x + drawerTerrainSideView.w/2 + point_y_rand;
    point_de_chute.y = drawerTerrainSideView.y;

    //point de chute de la balle
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &point_de_chute);
}

void drawInformations(){
    int texte_width = 200;
    int texte_height = 40;
    char str[20];
    char str2[20];
    char zoneChuteChaine[20] = "zone chute :";
    char zoneCanonChaine[20] = "zone canon :";
    drawString("informations :", window_width-texte_width, texte_height*0, texte_width, texte_height, 255, 255, 255, 255);
    drawString("e : new point chute", window_width-texte_width, texte_height*1, texte_width, texte_height, 255, 255, 255, 255);
    sprintf(str2, "%d", zone_chute);
    strcat(zoneChuteChaine, str2);
    drawString(zoneChuteChaine, window_width-texte_width, texte_height*2, texte_width, texte_height, 255, 255, 255, 255);
    drawString("r : new canon", window_width-texte_width, texte_height*4, texte_width, texte_height, 255, 255, 255, 255);
    sprintf(str, "%d", zone_canon);
    strcat(zoneCanonChaine, str);
    drawString(zoneCanonChaine, window_width-texte_width, texte_height*5, texte_width, texte_height, 255, 255, 255, 255);
}

void drawTrajectoireTopView(){
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderDrawLine(renderer, terrain.x+canon.x, terrain.y+canon.y, point_de_chute.x, point_de_chute.y);
}

void drawBall(){
    /*int i;
    SDL_Rect rect_life;
    SDL_Rect rect_mana;
    rect_life.h = 50;
    rect_life.w = 50;
    rect_life.x = 100;
    rect_life.y = 100; 

    rect_mana.h = 50;
    rect_mana.w = 50;
    rect_mana.x = 100;
    rect_mana.y = 200;*/
    
    //draw hero life
    /*SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for(i=0; i<hero.life; i++){
        SDL_RenderFillRect(renderer, &rect_life);
        rect_life.x = rect_life.x + rect_life.w + 20;
    }

    //draw hero mana
    */
    
}


void mainLoop(){
    createWindow();
    initCanon();
    initTerrain();
    initPointDeChute();

    pthread_t eventThread;
    if (pthread_create(&eventThread, NULL, eventLoop, NULL) != 0){
        printf("Couldn't create thread.");
        exit(EXIT_FAILURE);
    }

    while (running){
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        //top view
        drawTerrainTopView();
        drawCanonTopView();
        drawPointDeChuteTopView();
        drawTrajectoireTopView();
        zone_canon = getZone(terrain.x, terrain.y, terrain.w, terrain.h);
        zone_chute = getZoneChute(terrain.x, terrain.y, terrain.w, terrain.h);

        //side view
        drawTerrainSideView();
        drawCanonSideView();
        drawPointDeChuteSideView();
    
        //
        drawInformations();
       
        SDL_RenderPresent(renderer);
    }
    endSDL();
} 