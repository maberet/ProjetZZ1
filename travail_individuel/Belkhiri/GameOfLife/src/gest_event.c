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
                    case SDLK_x:
                        running = 0;
                        continue;
                    
                    default:
                        game_state = GAME;
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
                
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_x:
                        running = 0;
                        continue;

                    case SDLK_RIGHT:
                        updateMap(map);
                        continue;

                    case SDLK_s:
                        writeMap(map, "map.txt");
                        printf("Saved map to map.txt\n");
                    
                    default:
                        continue;
                    }
                    continue;

                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT){
                        int x = event.button.x;
                        int y = event.button.y;
                        // change value of map[x][y]
                        if (x < ScreenDimension.w/2 - MAPSIZE*CELLSIZE/2 + MAPSIZE*CELLSIZE && x > ScreenDimension.w/2 - MAPSIZE*CELLSIZE/2){
                            int i = (x - ScreenDimension.w/2 + MAPSIZE*CELLSIZE/2)/CELLSIZE;
                            int j = (y)/CELLSIZE;
                            map[j][i] = !map[j][i];
                        }
                    }
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
            case GAME : gestGame();break;
            default:printf("game state fault");break;
        }
    }
    return NULL;
}