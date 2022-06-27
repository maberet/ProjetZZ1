#include "gest_event.h"


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

                    default:
                        continue;
                    }
                    break;

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
            //case GAME : gestGame();break;
            default:printf("game state fault");break;
        }
    }
    return NULL;
} 