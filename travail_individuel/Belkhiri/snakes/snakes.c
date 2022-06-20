#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define NB_BLOCS 4
#define NB_SNAKES 10

typedef struct bloc {
    int p1[2];
    int p2[2];
    int p3[2];
    int p4[2];
} bloc_t;

typedef struct snake {
    bloc_t * blocs[NB_BLOCS];
} snake_t;


int running = 1;

void end_sdl(char ok,                                               
             char const* msg,                                       
             SDL_Window* window,                                    
             SDL_Renderer* renderer) {                              
  char msg_formated[255];                                                         
  int l;                                                                          

  if (!ok) {
    strncpy(msg_formated, msg, 250);                                              
    l = strlen(msg_formated);                                                     
    strcpy(msg_formated + l, " : %s\n");                                          

    SDL_Log(msg_formated, SDL_GetError());                                        
  }                                                                               

  if (renderer != NULL) {
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
  }
  if (window != NULL)   {
    SDL_DestroyWindow(window);
    window= NULL;
  }

  SDL_Quit();                                                                     

  if (!ok) {      
    exit(EXIT_FAILURE);                                                           
  }                                                                               
}                                                                                 

void draw_snake(SDL_Renderer* renderer, snake_t * Snake_to_draw) {
    for (int i = 0; i < NB_BLOCS; i++) {
        SDL_SetRenderDrawColor(renderer, rand() % 255, rand() % 255, rand() % 255, 255);
        SDL_RenderDrawLine(renderer, Snake_to_draw->blocs[i]->p1[0], Snake_to_draw->blocs[i]->p1[1], Snake_to_draw->blocs[i]->p2[0], Snake_to_draw->blocs[i]->p2[1]);
        SDL_RenderDrawLine(renderer, Snake_to_draw->blocs[i]->p2[0], Snake_to_draw->blocs[i]->p2[1], Snake_to_draw->blocs[i]->p3[0], Snake_to_draw->blocs[i]->p3[1]);
        SDL_RenderDrawLine(renderer, Snake_to_draw->blocs[i]->p3[0], Snake_to_draw->blocs[i]->p3[1], Snake_to_draw->blocs[i]->p4[0], Snake_to_draw->blocs[i]->p4[1]);
        SDL_RenderDrawLine(renderer, Snake_to_draw->blocs[i]->p4[0], Snake_to_draw->blocs[i]->p4[1], Snake_to_draw->blocs[i]->p1[0], Snake_to_draw->blocs[i]->p1[1]);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    }
}

snake_t * init_snake(int x, int y, int h, int w) {
    snake_t * snake = malloc(sizeof(snake_t));
    for (int i = 0; i < NB_BLOCS; i++) {
        snake->blocs[i] = malloc(sizeof(bloc_t));
        if (i == 0){
        snake->blocs[i]->p1[0] = x;
        snake->blocs[i]->p1[1] = y;
        snake->blocs[i]->p2[0] = x;
        snake->blocs[i]->p2[1] = y+h;
        snake->blocs[i]->p3[0] = x+w;
        snake->blocs[i]->p3[1] = y+h;
        snake->blocs[i]->p4[0] = x+w;
        snake->blocs[i]->p4[1] = y;
        }
        else {
            snake->blocs[i]->p1[0] = snake->blocs[i-1]->p2[0];
            snake->blocs[i]->p1[1] = snake->blocs[i-1]->p2[1];
            snake->blocs[i]->p2[0] = snake->blocs[i]->p1[0];
            snake->blocs[i]->p2[1] = snake->blocs[i]->p1[1] + h/(i+2);
            snake->blocs[i]->p4[0] = snake->blocs[i-1]->p3[0];
            snake->blocs[i]->p4[1] = snake->blocs[i-1]->p3[1];
            snake->blocs[i]->p3[0] = snake->blocs[i]->p4[0];
            snake->blocs[i]->p3[1] = snake->blocs[i]->p4[1] + h/(i+2);
        }
    }
    return snake;
}

void rotate_bloc(float angle, bloc_t * bloc){
    float x1 = bloc->p1[0];
    float y1 = bloc->p1[1];
    float x2 = bloc->p2[0];
    float y2 = bloc->p2[1];
    float x3 = bloc->p3[0];
    float y3 = bloc->p3[1];
    float x4 = bloc->p4[0];
    float y4 = bloc->p4[1];
    // middle is center of rotation
    float middle_x = (x1 + x4)/2;
    float middle_y = (y1 + y4)/2;

    float x1_new = x1 - middle_x;
    float y1_new = y1 - middle_y;
    float x2_new = x2 - middle_x;
    float y2_new = y2 - middle_y;
    float x3_new = x3 - middle_x;
    float y3_new = y3 - middle_y;
    float x4_new = x4 - middle_x;
    float y4_new = y4 - middle_y;
    // rotate
    x1_new = x1_new * cos(angle) - y1_new * sin(angle);
    y1_new = x1_new * sin(angle) + y1_new * cos(angle);
    x2_new = x2_new * cos(angle) - y2_new * sin(angle);
    y2_new = x2_new * sin(angle) + y2_new * cos(angle);
    x3_new = x3_new * cos(angle) - y3_new * sin(angle);
    y3_new = x3_new * sin(angle) + y3_new * cos(angle);
    x4_new = x4_new * cos(angle) - y4_new * sin(angle); 
    y4_new = x4_new * sin(angle) + y4_new * cos(angle);
    // move back
    x1_new = x1_new + middle_x;
    y1_new = y1_new + middle_y;
    x2_new = x2_new + middle_x;
    y2_new = y2_new + middle_y;
    x3_new = x3_new + middle_x;
    y3_new = y3_new + middle_y;
    x4_new = x4_new + middle_x;
    y4_new = y4_new + middle_y;
    // assign new values
    bloc->p1[0] = x1_new;
    bloc->p1[1] = y1_new;
    bloc->p2[0] = x2_new;
    bloc->p2[1] = y2_new;
    bloc->p3[0] = x3_new;
    bloc->p3[1] = y3_new;
    bloc->p4[0] = x4_new;
    bloc->p4[1] = y4_new;
}

float random_angle_radian() {
    return (float)rand() / (float)RAND_MAX * 2 * M_PI;
}

void move_bloc(int x, int y, bloc_t * bloc){
    bloc->p1[0] += x;
    bloc->p1[1] += y;
    bloc->p2[0] += x;
    bloc->p2[1] += y;
    bloc->p3[0] += x;
    bloc->p3[1] += y;
    bloc->p4[0] += x;
    bloc->p4[1] += y;
}

void move_snake(int x, int y, snake_t * snake){
    for (int i = NB_BLOCS-1; i >= 0; i--) {
        move_bloc(x, y, snake->blocs[i]);
    }
}

void rotate_snake(float angle, snake_t * snake) {
    for (int i = 0; i < NB_BLOCS; i++) {
        if (i == 0){
            rotate_bloc(angle, snake->blocs[i]);
        }
        else {
            rotate_bloc(angle, snake->blocs[i]);
            snake->blocs[i]->p1[0] = snake->blocs[i-1]->p2[0];
            snake->blocs[i]->p1[1] = snake->blocs[i-1]->p2[1];
            snake->blocs[i]->p4[0] = snake->blocs[i-1]->p3[0];
            snake->blocs[i]->p4[1] = snake->blocs[i-1]->p3[1];
        }
    }
}

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

  SDL_Window* window = NULL;
  SDL_Renderer* renderer = NULL;

  SDL_DisplayMode screen;

  /*********************************************************************************************************************/  
  /*                         Initialisation de la SDL  + gestion de l'échec possible                                   */
  if (SDL_Init(SDL_INIT_VIDEO) != 0) end_sdl(0, "ERROR SDL INIT", window, renderer);

  SDL_GetCurrentDisplayMode(0, &screen);
  printf("Résolution écran\n\tw : %d\n\th : %d\n",
         screen.w, screen.h);

  /* Création de la fenêtre */
  window = SDL_CreateWindow("Premier dessin",
                            SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, screen.w,
                            screen.h,
                            SDL_WINDOW_OPENGL);
  if (window == NULL) end_sdl(0, "ERROR WINDOW CREATION", window, renderer);

  /* Création du renderer */
  renderer = SDL_CreateRenderer(window, -1,
                                SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL) end_sdl(0, "ERROR RENDERER CREATION", window, renderer);

    snake_t * snakeList[NB_SNAKES];
    for (int i = 0; i < NB_SNAKES; i++) {
        // random int between 0 and WIDTH

        snakeList[i] = init_snake(rand()%screen.w, rand()%screen.h, rand()%200, rand()%50);
    }

    for (int i=0; i< 50; i++){
        for (int j=0; j<NB_SNAKES; j++){
            float angle = 0.2 * pow(-1, 1 + rand()%2);
            rotate_snake(angle, snakeList[j]);
            move_snake(rand()%10, rand()%10, snakeList[j]);
            draw_snake(renderer, snakeList[j]);            
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(100);
        SDL_RenderClear(renderer);
    }



  /* on referme proprement la SDL */
  end_sdl(1, "Normal ending", window, renderer);
  return EXIT_SUCCESS;
}