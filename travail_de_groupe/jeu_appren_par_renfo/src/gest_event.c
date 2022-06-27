#include "gest_event.h"


int Keys[NB_KEYS];

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



void gestGame(){
    SDL_Event event;
    while (SDL_PollEvent(&event)){
            switch(event.type)
            {   
                case SDL_QUIT:
                    running = 0;
                    break;

                case SDL_MOUSEMOTION:
                    if (event.motion.xrel > 0){
                        player.angle += 0.01;
                        if (player.angle > 2*pi) player.angle -= 2*pi;
                    }
                    if (event.motion.xrel < 0){
                        player.angle -= 0.01;
                        if (player.angle < 0) player.angle += 2*pi;
                    }
                    if (event.motion.yrel > 0){
                        player.viewAngle -= 1;
                    }
                    if (event.motion.yrel < 0){
                        player.viewAngle += 1;
                    }
                    player.deltax = cos(player.angle);
                    player.deltay = sin(player.angle);

                case SDL_KEYUP:
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            running = 0;
                            continue;
                            
                        case SDLK_z:
                            Keys[2] = 1;
                            continue;

                        case SDLK_d:
                            Keys[3] = 1;
                            continue;

                        case SDLK_s:
                            Keys[0] = 1;
                            continue;

                        case SDLK_q:
                            Keys[1] = 1;
                            continue;


                    default:
                        continue;
                    }

                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_z:
                            Keys[2] = 0;
                            continue;

                        case SDLK_d:
                            Keys[3] = 0;
                            continue;

                        case SDLK_s:
                            Keys[0] = 0;
                            continue;

                        case SDLK_q:
                            Keys[1] = 0;
                            continue;
                    }

                default:
                    continue;

                
            }
    }
    managePlayer();
    SDL_Delay(5);
}


void *EventLoop(void *arg){
    while(running){
        switch(game_state){
            case MENU : gestMenu();break;
            case GAME : gestGame();break;
            default:printf("game state fault");break;
        }
    }
    return NULL;
} 