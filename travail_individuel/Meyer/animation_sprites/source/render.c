#include "render.h"

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;
SDL_Rect my_rect;

int width = 700;
int height = 700;

int texture_width;
int texture_height;

void initSDL2()
{

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        "Animation sprites",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_RESIZABLE);

    if (window == 0)
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
    }

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED);

    if (renderer == 0)
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }
}

void afficherEcran()
{

    SDL_SetRenderDrawColor(renderer, 0, 16, 158, 0);
    SDL_RenderClear(renderer);

    texture = load_texture_from_image("assets/background.png", window, renderer);
    SDL_QueryTexture(texture, NULL, NULL, &texture_width, &texture_height);
    my_rect.x = 0;
    my_rect.y = 0;
    my_rect.w = texture_width;
    my_rect.h = texture_height;
    SDL_RenderCopy(renderer, texture, &my_rect, NULL);

    SDL_RenderPresent(renderer);
    SDL_Delay(20);
    SDL_RenderClear(renderer);
}

void destroySDL2()
{
    SDL_DestroyTexture(texture);
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_Texture *load_texture_from_image(char *file_image_name, SDL_Window *window, SDL_Renderer *renderer)
{
    SDL_Surface *my_image = NULL;   // Variable de passage
    SDL_Texture *my_texture = NULL;

    my_image = IMG_Load(file_image_name); // Chargement de l'image dans la surface
                                          // image=SDL_LoadBMP(file_image_name); fonction standard de la SDL,
                                          // uniquement possible si l'image est au format bmp */
    if (my_image == NULL){
        end_sdl(0, "Chargement de l'image impossible", window, renderer);
    }
    my_texture = SDL_CreateTextureFromSurface(renderer, my_image); // Chargement de l'image de la surface vers la texture
    SDL_FreeSurface(my_image);                                     // la SDL_Surface ne sert que comme élément transitoire
    if (my_texture == NULL){
        end_sdl(0, "Echec de la transformation de la surface en texture", window, renderer);
    }
    return my_texture;
}