#include "render.h"

SDL_Window *window;
SDL_Renderer *renderer;
SDL_DisplayMode ScreenDimension;

TTF_Font *RobotoFont;

SDL_Surface * character_walk_surface;
SDL_Surface * character_fall_surface;
SDL_Surface * character_attack_surface;

SDL_Texture * character_walk_texture;
SDL_Texture * character_fall_texture;
SDL_Texture * character_attack_texture;

SDL_Surface * parallax1_surface;
SDL_Surface * parallax2_surface;
SDL_Surface * parallax3_surface;

SDL_Texture * parallax1_texture;
SDL_Texture * parallax2_texture;
SDL_Texture * parallax3_texture;

int character_walk_w;
int character_walk_h;

int character_fall_w;
int character_fall_h;

int character_attack_w;
int character_attack_h;

void CreateWindow(){

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

    SDL_Rect parallax2_rect;
    parallax2_rect.x = step;
    parallax2_rect.y = 0;
    int parallax2_w, parallax2_h;
    SDL_QueryTexture(parallax2_texture, NULL, NULL, &parallax2_w, &parallax2_h);
    parallax2_rect.w = ScreenDimension.w;
    parallax2_rect.h = ScreenDimension.h;

    SDL_Rect parallax3_rect;
    parallax3_rect.x = 4 * step;
    parallax3_rect.y = 0;
    int parallax3_w, parallax3_h;
    SDL_QueryTexture(parallax3_texture, NULL, NULL, &parallax3_w, &parallax3_h);
    parallax3_rect.w = ScreenDimension.w/10;
    parallax3_rect.h = ScreenDimension.h;


    SDL_RenderCopyEx(renderer, parallax1_texture, NULL, NULL, 0, NULL, SDL_FLIP_NONE);
    SDL_RenderCopyEx(renderer, parallax2_texture, &parallax2_rect, NULL, 0, NULL, SDL_FLIP_NONE);
    SDL_RenderCopyEx(renderer, parallax3_texture, &parallax3_rect, NULL, 0, NULL, SDL_FLIP_NONE);

    SDL_Rect character_walk_rect;

    character_walk_rect.x = 4 * step;
    character_walk_rect.y = 100;
    character_walk_rect.w = 100;
    character_walk_rect.h = 250;



    SDL_Rect destRect;
    if (step < 50){
        destRect.w = character_walk_w/8;
        destRect.h = character_walk_h;
        destRect.x = (destRect.w * step)%character_walk_w;
        destRect.y = 0;
        SDL_RenderCopyEx(renderer, character_walk_texture, &destRect, &character_walk_rect, 0, NULL, SDL_FLIP_NONE);
    }
    else if (step >= 50 && step < 100){
        destRect.w = character_fall_w/2;
        destRect.h = character_fall_h;
        destRect.x = (destRect.w * (step - 50))%character_fall_w;
        destRect.y = 0;
        character_walk_rect.y = 100 + (step - 50) * 5;
        SDL_RenderCopyEx(renderer, character_fall_texture, &destRect, &character_walk_rect, 0, NULL, SDL_FLIP_NONE);
    }
    else if (step >= 100 && step < 150){
        destRect.w = character_walk_w/8;
        destRect.h = character_walk_h;
        destRect.x = (destRect.w * step)%character_walk_w;
        destRect.y = 0;
        character_walk_rect.y = 100 + (50) * 5;
        SDL_RenderCopyEx(renderer, character_walk_texture, &destRect, &character_walk_rect, 0, NULL, SDL_FLIP_NONE);
    }
    else if (step >= 150 && step < 200){
        destRect.w = character_attack_w/4;
        destRect.h = character_attack_h;
        destRect.x = (destRect.w * (step - 150))%character_attack_w;
        destRect.y = 0;
        character_walk_rect.y = 100 + (50) * 5;
        character_walk_rect.w *= 2;
        character_walk_rect.h *= 1.5;
        SDL_RenderCopyEx(renderer, character_attack_texture, &destRect, &character_walk_rect, 0, NULL, SDL_FLIP_NONE);
    }
    else {
        running = 0;
    }


    SDL_RenderPresent(renderer);
    SDL_Delay(100);
}


void MainLoop(){
    CreateWindow();
    int step = 0;

    character_walk_surface = IMG_Load("Res/king_run_spritesheet.png");
    character_fall_surface = IMG_Load("Res/king_fall_spritesheet.png");
    character_attack_surface = IMG_Load("Res/king_attack_spritesheet.png");
    parallax1_surface = IMG_Load("Res/parallax1.png");
    parallax2_surface = IMG_Load("Res/parallax2.png");
    parallax3_surface = IMG_Load("Res/parallax3.png");

    character_walk_texture = SDL_CreateTextureFromSurface(renderer, character_walk_surface);
    character_fall_texture = SDL_CreateTextureFromSurface(renderer, character_fall_surface);
    character_attack_texture = SDL_CreateTextureFromSurface(renderer, character_attack_surface);
    parallax1_texture = SDL_CreateTextureFromSurface(renderer, parallax1_surface);
    parallax2_texture = SDL_CreateTextureFromSurface(renderer, parallax2_surface);
    parallax3_texture = SDL_CreateTextureFromSurface(renderer, parallax3_surface);

    SDL_FreeSurface(character_walk_surface);
    SDL_FreeSurface(character_fall_surface);
    SDL_FreeSurface(character_attack_surface);
    SDL_FreeSurface(parallax1_surface);
    SDL_FreeSurface(parallax2_surface);
    SDL_FreeSurface(parallax3_surface);

    SDL_QueryTexture(character_walk_texture, NULL, NULL, &character_walk_w, &character_walk_h);
    SDL_QueryTexture(character_fall_texture, NULL, NULL, &character_fall_w, &character_fall_h);
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