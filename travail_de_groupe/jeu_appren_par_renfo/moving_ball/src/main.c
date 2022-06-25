#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <time.h>
#define M_PI 3.14159265358979323846
#define MAPSIZE 20

int running = 1;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;


void initSDL(){
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    if(TTF_Init() == -1){
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        exit(1);
    }
    if(IMG_Init(IMG_INIT_PNG) < 0){
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        exit(1);
    }
}

void createWindow(){
    window = SDL_CreateWindow("moving ball", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, SDL_WINDOW_SHOWN);
    if(window == NULL){
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == NULL){
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
}

void endSDL(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

void readMapFromFile(char * filename, int map[][MAPSIZE]){
    FILE * fp;
    fp = fopen(filename, "r");
    if(fp == NULL){
        printf("Error opening file\n");
        exit(1);
    }
    int i, j;
    for(i = 0; i < MAPSIZE; i++){
        for(j = 0; j < MAPSIZE; j++){
            fscanf(fp, "%d", &map[i][j]);
        }
    }
    fclose(fp);
}

typedef struct Ball {
    int x;
    int y;
    int radius;
    int weight;
    int isGrounded;
} Ball_t;

void printMap(int map[][MAPSIZE]){
    int i, j;
    for(i = 0; i < MAPSIZE; i++){
        for(j = 0; j < MAPSIZE; j++){
            printf("%d ", map[i][j]);
        }
        printf("\n");
    }
}

void drawMap(SDL_Renderer * renderer, int map[][MAPSIZE], Ball_t * ball){
    int i, j;
    SDL_Rect rect = {0, 0, 800/MAPSIZE, 800/MAPSIZE};
    for(i = 0; i < MAPSIZE; i++){
        for(j = 0; j < MAPSIZE; j++){
            if (i == ball->x && j == ball->y){
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderFillRect(renderer, &rect);
            }
            else {
                if (map[i][j] == 2){
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                    SDL_RenderFillRect(renderer, &rect);
                }
                else {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderFillRect(renderer, &rect);
                }
            }
            rect.x += 800/MAPSIZE;
        }
        rect.y += 800/MAPSIZE;
        rect.x = 0;
    }
}

#define GRAVITY 1

int hitAngle;
int hitIntensity;

void moveBall(Ball_t * ball, int choice){
    switch (choice)
    {
    case 0:
        ball->x += 1;
        break;

    case 1:
        ball->x -= 1;
        break;

    case 2:
        ball->y += 1;
        break;

    case 3:
        ball->y -= 1;
        break;
    }
}

float *** allocateAndInitiateQ(){
    float *** q = malloc(sizeof(int **) * MAPSIZE);
    int i, j, k;
    for(i = 0; i < MAPSIZE; i++){
        q[i] = malloc(sizeof(float *) * MAPSIZE);
        for(j = 0; j < MAPSIZE; j++){
            q[i][j] = malloc(sizeof(float) * 4);

            if (i > 0 && i < MAPSIZE - 1 && j > 0 && j < MAPSIZE - 1){
                for(k = 0; k < 4; k++){
                    q[i][j][k] = -1;
                }
            }
            else if (i == 0 && j == 0){
                int kPossibleValues[2] = {0, 2};
                for(k = 0; k < 2; k++){
                    q[i][j][kPossibleValues[k]] = -1;
                }
                q[i][j][3] = -INFINITY;
                q[i][j][1] = -INFINITY;
            }
            else if (i == 0 && j == MAPSIZE - 1){
                int kPossibleValues[2] = {1, 2};
                for(k = 0; k < 2; k++){
                    q[i][j][kPossibleValues[k]] = -1;
                }
                q[i][j][0] = -INFINITY;
                q[i][j][3] = -INFINITY;
            }
            else if (i == MAPSIZE - 1 && j == 0){
                int kPossibleValues[2] = {0, 3};
                for(k = 0; k < 2; k++){
                    q[i][j][kPossibleValues[k]] = -1;
                }
                q[i][j][1] = -INFINITY;
                q[i][j][2] = -INFINITY;
            }
            else if (i == MAPSIZE - 1 && j == MAPSIZE - 1){
                int kPossibleValues[2] = {1, 3};
                for(k = 0; k < 2; k++){
                    q[i][j][kPossibleValues[k]] = -1;
                }
                q[i][j][0] = -INFINITY;
                q[i][j][2] = -INFINITY;
            }
            else if (j == MAPSIZE - 1){
                int kPossibleValues[3] = {1, 2, 3};
                for(k = 0; k < 3 ;k++){
                    q[i][j][kPossibleValues[k]] = -1;
                }
                q[i][j][0] = -INFINITY;
            }
            else if (i == MAPSIZE - 1){
                int kPossibleValues[3] = {0, 1, 3};
                for(k = 0; k < 3; k++){
                    q[i][j][kPossibleValues[k]] = -1;
                }
                q[i][j][2] = -INFINITY;
            }
            else if (j == 0){
                int kPossibleValues[3] = {0, 2, 3};
                for(k = 0; k < 3; k++){
                    q[i][j][kPossibleValues[k]] = -1;
                }
                q[i][j][1] = -INFINITY;
            }
            else if (i == 0){
                int kPossibleValues[3] = {0, 1, 2};
                for(k = 0; k < 3; k++){
                    q[i][j][kPossibleValues[k]] = -1;
                }
                q[i][j][3] = -INFINITY;
            }
        }
    }
    return q;
}

int argmax(float * arr, int size){
    int i;
    float max = arr[0];
    int maxIndex = 0;
    printf("argmax:  %f ", arr[0]);
    for(i = 1; i < size; i++){
        printf("%f ", arr[i]);
        if (arr[i] > max){
            max = arr[i];
            maxIndex = i;
        }
    }
    printf("\n");
    return maxIndex;
}

int take_action(Ball_t * ball, float *** Q, float eps){
    int action;
    int proba = rand() % 100;
    if (proba < eps * 100){
        if (ball->x > 0 && ball->x < MAPSIZE - 1 && ball->y > 0 && ball->y < MAPSIZE - 1){
            action = rand() % 4;
        }
        else if (ball->x == 0 && ball->y > 0 && ball->y < MAPSIZE - 1){
            int possibleActions[3] = {0, 3, 2};
            action = possibleActions[rand() % 3];
        }
        else if (ball->x == MAPSIZE - 1 && ball->y > 0 && ball->y < MAPSIZE - 1){
            int possibleActions[3] = {1, 3, 2};
            action = possibleActions[rand() % 3];
        }
        else if (ball->y == 0 && ball->x > 0 && ball->x < MAPSIZE - 1){
            int possibleActions[3] = {0, 1, 2};
            action = possibleActions[rand() % 3];
        }
        else if (ball->y == MAPSIZE - 1 && ball->x > 0 && ball->x < MAPSIZE - 1){
            int possibleActions[3] = {0, 1, 3};
            action = possibleActions[rand() % 3];
        }
        else if (ball->x == 0 && ball->y == 0){
            int possibleActions[2] = {0, 2};
            action = possibleActions[rand() % 2];
        }
        else if (ball->x == 0 && ball->y == MAPSIZE - 1){
            int possibleActions[2] = {0, 3};
            action = possibleActions[rand() % 2];
        }
        else if (ball->x == MAPSIZE - 1 && ball->y == 0){
            int possibleActions[2] = {1, 2};
            action = possibleActions[rand() % 2];
        }
        else if (ball->x == MAPSIZE - 1 && ball->y == MAPSIZE - 1){
            int possibleActions[2] = {1, 3};
            action = possibleActions[rand() % 2];
        }
        else{
            action = rand() % 4;
        }
    }
    else{
        action = argmax(Q[ball->y][ball->x],4);
        //printf("wtf");
    }
    return action;
}

int setReward(Ball_t * ball, int map[][MAPSIZE]){
    if (map[ball->y][ball->x] == 1){
        return 0;
    }
    else if (map[ball->y][ball->x] == 2){
        return 1;
    }
    else if (map[ball->y][ball->x] == 3){
        return -2;
    }
    else{
        return -1;
    }
}

void printQ(float *** Q){
    int i, j, k;
    for(i = 0; i < MAPSIZE; i++){
        for(j = 0; j < MAPSIZE; j++){
            printf("Q for %d %d: ", i, j);
            for(k = 0; k < 4; k++){
                printf("%f ", Q[i][j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }
}

void writeQ(float *** Q){
    int i, j, k;
    FILE * fp = fopen("q.txt", "w+");
    for(i = 0; i < MAPSIZE; i++){
        for(j = 0; j < MAPSIZE; j++){
            for(k = 0; k < 4; k++){
                fprintf(fp, "%f ", Q[i][j][k]);
            }
            fprintf(fp, "\n");
        }
        fprintf(fp, "\n");
    }
    fflush(fp);
    fclose(fp);
}

void readQ(float *** Q){
    int i, j, k;
    FILE * fp = fopen("q.txt", "r");
    for(i = 0; i < MAPSIZE; i++){
        for(j = 0; j < MAPSIZE; j++){
            for(k = 0; k < 4; k++){
                fscanf(fp, "%f ", &Q[i][j][k]);
            }
            fscanf(fp, "\n");
        }
        fscanf(fp, "\n");
    }
    fflush(fp);
    fclose(fp);
}

typedef struct path_t{
    int x;
    int y;
    struct path_t * next;
} path_t;

// chain list to track the path taken

void insertPointToPath(path_t ** head, int x, int y){
    path_t * newPoint = (path_t *)malloc(sizeof(path_t));
    newPoint->x = x;
    newPoint->y = y;
    newPoint->next = *head;
    *head = newPoint;
}

int checkIfPointInPath(path_t * head, int x, int y){
    path_t * current = head;
    while(current != NULL){
        if (current->x == x && current->y == y){
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void freePath(path_t * head){
    path_t * current = head;
    while(current != NULL){
        path_t * temp = current;
        current = current->next;
        free(temp);
    }
}

int main(){
    Ball_t ball;
    ball.weight = 100;
    ball.isGrounded = 1;

    path_t * path = NULL;
    path = (path_t *)malloc(sizeof(path_t));

    srand(time(NULL));

    int map[MAPSIZE][MAPSIZE];
    readMapFromFile("map.txt", map);
    //printMap(map);

    float *** Q = allocateAndInitiateQ();
    
    for (int i = 0; i < 1000; i++){
        freePath(path);
        path = (path_t *)malloc(sizeof(path_t));
        ball.x = rand() % MAPSIZE;
        ball.y = rand() % MAPSIZE;
        while (running){
            if (map[ball.y][ball.x] == 2){
                printf("%d : found at %d %d\n", i, ball.x, ball.y);
                running = 0;
            }

            insertPointToPath(&path, ball.x, ball.y);
            
            int action = take_action(&ball, Q, 0.5);
            printf("[%d] Action: %d\n", i, action);
            int reward = setReward(&ball, map);

            printf("[%d] Reward: %d\n", i, reward);

            printf("Ball: %d %d\n", ball.x, ball.y);
            fflush(stdout);

            Ball_t * nextBall = malloc(sizeof(Ball_t));
            nextBall->x = ball.x;
            nextBall->y = ball.y;
            nextBall->weight = ball.weight;
            nextBall->isGrounded = ball.isGrounded;


            moveBall(nextBall, action);
            
            if (checkIfPointInPath(path, nextBall->x, nextBall->y)){
                reward = -1;
            }

            printf("Next Ball: %d, %d\n", nextBall->x, nextBall->y);

            int nextAction = argmax(Q[nextBall->y][nextBall->x],4);


            Q[ball.y][ball.x][action] =  Q[ball.y][ball.x][action] + 0.1 * (reward + 0.9 * Q[nextBall->y][nextBall->x][nextAction] - Q[ball.y][ball.x][action]);
            printf("Q: %f\n", Q[ball.y][ball.x][action] + 0.1 * (reward + 0.9 * Q[nextBall->y][nextBall->x][nextAction] - Q[ball.y][ball.x][action]));

            moveBall(&ball, action);
        }
        running = 1;
    }
    //printQ(Q);
    writeQ(Q);

    readQ(Q);
    // use Q after training
    ball.x = rand() % MAPSIZE;
    ball.y = rand() % MAPSIZE;
    // use Q

    createWindow();

    int nextIteration = 0;
    
    while (running){
        SDL_Event event;
        while (SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE){
                        running = 0;
                    }
                    if (event.key.keysym.sym == SDLK_UP){
                        nextIteration = 1;
                    }
                    break;
                default:
                    break;
            }
        }
        if (nextIteration){
            nextIteration = 0;
            SDL_RenderClear(renderer);
            drawMap(renderer, map, &ball);
            SDL_RenderPresent(renderer);
            if (map[ball.y][ball.x] == 2){
                printf("found at final %d %d\n", ball.x, ball.y);
                running = 0;
            }
            int action = take_action(&ball, Q, 0);
            printf("Action: %d\n", action);
            int reward = setReward(&ball, map);
            printf("Reward: %d\n", reward);

            moveBall(&ball, action);
            printf("Ball: %d %d\n", ball.x, ball.y);
            fflush(stdout);
            }
    }
    endSDL();
}