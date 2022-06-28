#include "gest_event.h"

SDL_Point mousePosition;

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

                    case SDLK_e:
                        initPointDeChute();
                        zone_chute = getZoneChute(terrain.x, terrain.y, terrain.w, terrain.h);
                        break;

                    case SDLK_r:
                        newCanon();
                        canon.zone = getZone(terrain.x, terrain.y, terrain.w, terrain.h);
                        break;
                        
                    default:
                        continue;
                    }
                    break;
                
                case SDL_MOUSEMOTION:
                    continue;

                case SDL_MOUSEBUTTONDOWN:
                    //                    
                    break;

                default:
                    continue;
            }
    }
    SDL_Delay(5);
}

void *eventLoop(void *arg){
    while(running){
        manageGame();
    }
    return NULL;
} 