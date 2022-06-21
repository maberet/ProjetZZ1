#include "render.h"

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;
SDL_Texture *texture2;
SDL_Texture *texture3;

int width = 700;
int height = 700;

int background_w;
int background_h;

int sun_w;
int sun_h;

SDL_Rect background;
SDL_Rect sun;
SDL_Rect destRect;
SDL_Rect destRect2;

SDL_Rect
        source = {0},            
        window_dimensions = {0}, 
        destination = {0},       
        state = {0}; 

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

void loadTexture()
{
    texture = load_texture_from_image("assets/background.png", window, renderer);
    texture2 = load_texture_from_image("assets/sun.png", window, renderer);
    texture3 = load_texture_from_image("assets/player-spritemap-v9.png", window, renderer);

    SDL_QueryTexture(texture, NULL, NULL, &background_w, &background_h);
    SDL_QueryTexture(texture2, NULL, NULL, &sun_w, &sun_h);
    SDL_QueryTexture(texture3, NULL, NULL, &source.w, &source.h);
}

void afficherEcran()
{
    // couleur de fond
    // SDL_SetRenderDrawColor(renderer, 0, 16, 158, 0);

    SDL_GetWindowSize(window, // Récupération des dimensions de la fenêtre
                      &window_dimensions.w,
                      &window_dimensions.h);
    

    background.x = 0;
    background.y = 0;
    background.w = width;
    background.h = height;

    sun.x = 400;
    sun.y = 50;
    sun.w = sun_w;
    sun.h = sun_h;

    destRect.w = background_w;
    destRect.h = background_h;
    destRect.x = 0;
    destRect.y = 0;

    destRect2.w = sun_w;
    destRect2.h = sun_h;
    destRect2.x = 0;
    destRect2.y = 0;

    int nb_images = 8;                   
    float zoom = 2;                      
    int offset_x = source.w / nb_images, // La largeur d'une vignette de l'image, marche car la planche est bien réglée
        offset_y = source.h / 4;         // La hauteur d'une vignette de l'image, marche car la planche est bien réglée

    state.x = 0;            
    state.y = 3 * offset_y; // On s'intéresse à la 4ème ligne, le bonhomme qui court
    state.w = offset_x;     // Largeur de la vignette
    state.h = offset_y;     // Hauteur de la vignette

    destination.w = offset_x * zoom; // Largeur du sprite à l'écran
    destination.h = offset_y * zoom; // Hauteur du sprite à l'écran

    destination.y = 500;

    int nb_it = 100;
    int speed = 9;
    for (int i = 0; i < nb_it; i++)
    {
        sun.x = sun.x - 3;
        destination.x = -100 + i*10 + speed;
        state.x += offset_x;
        state.x %= source.w;
        SDL_RenderClear(renderer);
        SDL_SetTextureAlphaMod(texture2, (1.0 - 1.0 * i / nb_it) * 255);
        SDL_RenderCopy(renderer, texture, &destRect, &background);
        SDL_RenderCopyEx(renderer, texture2, &destRect2, &sun, 0, NULL, SDL_FLIP_NONE);
        SDL_RenderCopy(renderer, texture3, &state, &destination);
        SDL_RenderPresent(renderer);
        SDL_Delay(20);
    }
    SDL_RenderClear(renderer);
}

void destroySDL2()
{
    SDL_DestroyTexture(texture3);
    SDL_DestroyTexture(texture2);
    SDL_DestroyTexture(texture);
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_Texture *load_texture_from_image(char *file_image_name, SDL_Window *window, SDL_Renderer *renderer)
{
    SDL_Surface *my_image = NULL; // Variable de passage
    SDL_Texture *my_texture = NULL;

    my_image = IMG_Load(file_image_name); // Chargement de l'image dans la surface
                                          // image=SDL_LoadBMP(file_image_name); fonction standard de la SDL,
                                          // uniquement possible si l'image est au format bmp */
    if (my_image == NULL)
    {
        end_sdl(0, "Chargement de l'image impossible", window, renderer);
    }
    my_texture = SDL_CreateTextureFromSurface(renderer, my_image); // Chargement de l'image de la surface vers la texture
    SDL_FreeSurface(my_image);                                     // la SDL_Surface ne sert que comme élément transitoire
    if (my_texture == NULL)
    {
        end_sdl(0, "Echec de la transformation de la surface en texture", window, renderer);
    }
    return my_texture;
}


