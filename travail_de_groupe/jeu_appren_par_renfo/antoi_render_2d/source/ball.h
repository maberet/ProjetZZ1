#ifndef BALL_H
#define BALL_H

typedef struct ball{
    int x;
    int y;
} ball_t;

extern ball_t ball;

void initBall();

#endif