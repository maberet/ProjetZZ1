#include "gest_event.h"

int hover[2];
int x;
int y;

int Keys[10];

void gestMenu(){
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
                        Keys[0] = 0;
                        break;
                    
                    case SDLK_DOWN:
                        Keys[1] = 0;
                        break;

                    case SDLK_LEFT:
                        Keys[2] = 0;
                        break;

                    case SDLK_RIGHT:
                        Keys[3] = 0;
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
                        Keys[0] = 1;
                        break;

                    case SDLK_DOWN:
                        Keys[1] = 1;
                        break;

                    case SDLK_LEFT:
                        Keys[2] = 1;
                        break;

                    case SDLK_RIGHT:
                        Keys[3] = 1;
                        break;

                    default:
                        continue;
                    }
                    break;
                
                case SDL_MOUSEMOTION:
                        x = (event.motion.x - (ScreenDimension.w - (MAPSIZE * CELLSIZE)) / 2) / CELLSIZE;
                        y = (event.motion.y - 0) / CELLSIZE;
                        hover[0] = x;
                        hover[1] = y;
                    continue;

                default:
                    continue;
            }
    }
    SDL_Delay(5);
}


void *EventLoop(void *arg){
    while(running){
        switch(game_state){
            case MENU : gestMenu();break;
            case GAME : gestMenu();break;
            default:printf("game state fault");break;
        }
    }
    return NULL;
} 