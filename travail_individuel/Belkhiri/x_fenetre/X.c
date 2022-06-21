#include <SDL2/SDL.h>
#include <stdio.h>


int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    int screen_width = 1079;
    int screen_height = 1920;

    SDL_Window * windows[20];

    int bounce_count = 0;
    int amplitude_bounce = 200;
    int left_most_position = 0;
    int top_most_position = 0;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("Error : SDL initialisation - %s\n", 
             SDL_GetError());
    exit(EXIT_FAILURE);
    }
        int pas_y = (screen_height)/20;
        int pas_x = (screen_width)/20;
        int last_drawy = 0;

    for  (int i = 0; i < 10; i++) {
        int draw_x = top_most_position + i * pas_x;
        int draw_y = left_most_position + i * pas_y;
        last_drawy = draw_y;
        windows[i] = SDL_CreateWindow("Window",
                                        draw_y , draw_x,  
                                        400, 300,
                                        SDL_WINDOW_SHOWN);
        if (windows[i] == NULL) {
                SDL_Log("Error : SDL window creation - %s\n", 
                        SDL_GetError());
                exit(EXIT_FAILURE);
        }
        SDL_Delay(50);
    }

    for  (int i = 10; i < 20; i++) {
        int draw_x = top_most_position + (i-10) * pas_x;
        int draw_y = last_drawy - (i-10) * pas_y;
        windows[i] = SDL_CreateWindow("Window",
                                    draw_y , draw_x,  
                                    400, 300,
                                    SDL_WINDOW_SHOWN);
        if (windows[i] == NULL) {
            SDL_Log("Error : SDL window creation - %s\n", 
                    SDL_GetError());
            exit(EXIT_FAILURE);
        }
        SDL_Delay(50);
    }

    SDL_Delay(100);

    while (bounce_count < 50) {

    
        for  (int i = 0; i < 10; i++) {
            int draw_x = top_most_position + i * pas_x;
            int draw_y = left_most_position + i * pas_y;
            last_drawy = draw_y;
            SDL_SetWindowPosition(windows[i],
                                draw_y , draw_x);

            //SDL_Delay(50);
        }

        for  (int i = 10; i < 20; i++) {
            int draw_x = top_most_position + (i-10) * pas_x;
            int draw_y = last_drawy - (i-10) * pas_y;
            SDL_SetWindowPosition(windows[i],
                                        draw_y , draw_x);
            //SDL_Delay(50);
        }

        SDL_Delay(100);


        bounce_count+=1;
        int signe = (bounce_count % 2 == 0) ? (1) : (-1);
        left_most_position = left_most_position + amplitude_bounce;
        amplitude_bounce = 200 * sin(bounce_count);
    }
    for (int i = 0; i < 20; i++) {
        SDL_DestroyWindow(windows[i]);
    }


    SDL_Delay(2000);

    SDL_Quit();

      return 0;
}