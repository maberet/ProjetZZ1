#include "gest_event.h"

int hover[2];
int x;
int y;

int keys[10];

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

                    case SDLK_UP:
                        keys[0] = 0;
                        break;
                    
                    case SDLK_DOWN:
                        keys[1] = 0;
                        break;

                    case SDLK_LEFT:
                        keys[2] = 0;
                        break;

                    case SDLK_RIGHT:
                        keys[3] = 0;
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
                    
                    case SDLK_UP:
                        keys[0] = 1;
                        break;

                    case SDLK_DOWN:
                        keys[1] = 1;
                        break;

                    case SDLK_LEFT:
                        keys[2] = 1;
                        break;

                    case SDLK_RIGHT:
                        keys[3] = 1;
                        break;

                    default:
                        continue;
                    }
                    break;
                
                case SDL_MOUSEMOTION:
                        x = (event.motion.x - (screenDimension.w - (MAPSIZE * CELLSIZE)) / 2) / CELLSIZE;
                        y = (event.motion.y - 0) / CELLSIZE;
                        hover[0] = x;
                        hover[1] = y;
                    continue;

                default:
                    continue;
            }
    }
    manageMovement();
    SDL_Delay(5);
}


void *eventLoop(void *arg){
    while(running){
        switch(gameState){
            case MENU : manageMenu();break;
            case GAME : manageMenu();break;
            default:printf("game state fault");break;
        }
    }
    return NULL;
} 