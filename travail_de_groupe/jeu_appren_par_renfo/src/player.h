#ifndef PLAYER_H
#define PLAYER_H

#define PLAYER_UP 0
#define PLAYER_DOWN 1
#define PLAYER_LEFT 2
#define PLAYER_RIGHT 3

typedef struct player{
    int x;
    int y;
    int speed;
    int isMoving;
    int direction;
    int HPMax;
    int currentHP;
    int coins;
    float angle;
} player_t;

extern player_t player;

void initPlayer();

#endif