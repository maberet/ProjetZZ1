#include "gest_event.h"

SDL_Point mousePosition;

int keys[NBKEYS];

void initKeys(){
    for (int i =0; i < NBKEYS; i++){
        keys[i] = 0;
    }
}

void manageMenu(){
    SDL_Event event;
    while (SDL_PollEvent(&event)){
            switch(event.type)
            {   
                case SDL_QUIT:
                    running = 0;
                    break;

                case SDL_KEYUP:
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_ESCAPE:
                        running = 0;
                        continue;

                    default:
                        continue;
                    }
                    break;
                
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_ESCAPE:
                        running = 0;
                        continue;

                    default:
                        continue;
                    }
                    break;
                
                case SDL_MOUSEMOTION:
                    mousePosition.x = event.motion.x;
                    mousePosition.y = event.motion.y;
                    continue;


                case SDL_MOUSEBUTTONDOWN:
                    if (SDL_PointInRect(&mousePosition, &buttonRect)){
                        gameState = GAME;
                    }
                    break;

                default:
                    continue;
            }
    }
    SDL_Delay(5);
}

void manageLost(){
    SDL_Event event;
    while (SDL_PollEvent(&event)){
            switch(event.type)
            {   
                case SDL_QUIT:
                    running = 0;
                    break;

                case SDL_KEYUP:
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_ESCAPE:
                        running = 0;
                        continue;

                    default:
                        continue;
                    }
                    break;
                
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_ESCAPE:
                        running = 0;
                        continue;

                    default:
                        continue;
                    }
                    break;
                
                case SDL_MOUSEMOTION:
                    mousePosition.x = event.motion.x;
                    mousePosition.y = event.motion.y;
                    continue;


                case SDL_MOUSEBUTTONDOWN:
                    if (SDL_PointInRect(&mousePosition, &playAgainButtonRect)){
                        initAll();
                        initKeys();
                        gameState = GAME;
                    }
                    if (SDL_PointInRect(&mousePosition, &quitButtonRect)){
                        running = 0;
                    }
                    break;

                default:
                    continue;
            }
    }
    SDL_Delay(5);
}

void manageGame(){
    SDL_Event event;
    while (SDL_PollEvent(&event)){
            switch(event.type)
            {   
                case SDL_QUIT:
                    running = 0;
                    break;

                case SDL_KEYUP:
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_ESCAPE:
                        running = 0;
                        continue;

                    case SDLK_z:
                        keys[PLAYER_UP] = 0;
                        break;
                    
                    case SDLK_s:
                        keys[PLAYER_DOWN] = 0;
                        break;

                    case SDLK_q:
                        keys[PLAYER_LEFT] = 0;
                        break;

                    case SDLK_d:
                        keys[PLAYER_RIGHT] = 0;
                        break;

                    default:
                        continue;
                    }
                    break;
                
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_ESCAPE:
                        running = 0;
                        continue;
                    
                    case SDLK_z:
                        keys[PLAYER_UP] = 1;
                        break;

                    case SDLK_s:
                        keys[PLAYER_DOWN] = 1;
                        break;

                    case SDLK_q:
                        keys[PLAYER_LEFT] = 1;
                        break;

                    case SDLK_d:
                        keys[PLAYER_RIGHT] = 1;
                        break;

                    default:
                        continue;
                    }
                    break;
                
                case SDL_MOUSEMOTION:
                    mousePosition.x = (event.motion.x - (screenDimension.w - (MAPSIZE * CELLSIZE)) / 2) / CELLSIZE;
                    mousePosition.y = (event.motion.y - 0) / CELLSIZE;
                    continue;

                case SDL_MOUSEBUTTONDOWN:
                    //clic on water
                    if(2 == map[mousePosition.y][mousePosition.x]){
                        fillWater(mousePosition.x, mousePosition.y);
                    }else{
                        pourWater(mousePosition.x, mousePosition.y);
                    }                    
                    break;

                default:
                    continue;
            }
    }
    updatePlayer();
    //burnTree(fireList);
    SDL_Delay(5);
}

void *eventLoop(void *arg){
    while(running){
        switch(gameState){
            case MENU : manageMenu();break;
            case GAME : manageGame();break;
            case LOSE : manageLost();break;
            case WON : manageLost();break;
            default:printf("game state fault");break;
        }
    }
    return NULL;
} 