#include "render.h"

SDL_Window *window;
SDL_Renderer *renderer;

TTF_Font *RobotoFont;

SDL_Surface * character_walk_surface;
SDL_Surface * character_jump_surface;
SDL_Surface * character_attack_surface;

SDL_Texture * character_walk_texture;
SDL_Texture * character_jump_texture;
SDL_Texture * character_attack_texture;

int character_walk_w;
int character_walk_h;

int character_jump_w;
int character_jump_h;

int character_attack_w;
int character_attack_h;

void CreateWindow(){
    SDL_DisplayMode ScreenDimension;

    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        printf("Couldn't create window.");
        exit(EXIT_FAILURE);
    }

    SDL_GetCurrentDisplayMode(0, &ScreenDimension);

    window = SDL_CreateWindow("Game Of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ScreenDimension.w, ScreenDimension.h, SDL_WINDOW_INPUT_GRABBED | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);

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

    RobotoFont = TTF_OpenFont("Res/Roboto-Black.ttf", 50);  

}

void Game(int step){
    SDL_RenderClear(renderer);
    SDL_Rect character_walk_rect;

    character_walk_rect.x = step;
    character_walk_rect.y = 0;
    character_walk_rect.w = 100;
    character_walk_rect.h = 250;

    if (step < 100){
        SDL_Rect destRect;
        destRect.w = character_walk_w/8;
        destRect.h = character_walk_h;
        destRect.x = (destRect.w * step)%character_walk_w;
        destRect.y = 0;
        SDL_RenderCopyEx(renderer, character_walk_texture, &destRect, &character_walk_rect, 0, NULL, SDL_FLIP_NONE);
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(100);
}


void MainLoop(){
    CreateWindow();
    int step = 0;

    character_walk_surface = IMG_Load("Res/king_run_spritesheet.png");
    character_jump_surface = IMG_Load("Res/king_jump_spritesheet.png");
    character_attack_surface = IMG_Load("Res/king_attack_spritesheet.png");

    character_walk_texture = SDL_CreateTextureFromSurface(renderer, character_walk_surface);
    character_jump_texture = SDL_CreateTextureFromSurface(renderer, character_jump_surface);
    character_attack_texture = SDL_CreateTextureFromSurface(renderer, character_attack_surface);

    SDL_FreeSurface(character_walk_surface);
    SDL_FreeSurface(character_jump_surface);
    SDL_FreeSurface(character_attack_surface);

    SDL_QueryTexture(character_walk_texture, NULL, NULL, &character_walk_w, &character_walk_h);
    SDL_QueryTexture(character_jump_texture, NULL, NULL, &character_jump_w, &character_jump_h);
    SDL_QueryTexture(character_attack_texture, NULL, NULL, &character_attack_w, &character_attack_h);

    unsigned int a = SDL_GetTicks();
    unsigned int b = SDL_GetTicks();
    double delta = 0;

    pthread_t eventThread;
    if (pthread_create(&eventThread, NULL, EventLoop, NULL) != 0){
        printf("Couldn't create thread.");
        exit(EXIT_FAILURE);
    }

    while (running){
        a = SDL_GetTicks();
        delta = (a - b) / 1000.0;
        if (delta > 1/FPS_TO_GET){
            b = a;
            switch (game_state){
                case GAME:
                    Game(step);
                    step++;
                    break;
            }
        }
        else {
            // fait dormir le thread pour garder des ressources
            usleep(1000 * (1/FPS_TO_GET - delta));
        }

    }
} 