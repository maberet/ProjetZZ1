#ifndef CANON_H
#define CANON_H

typedef struct canon{
    int x;
    int y;
    int width;
    int height;
    int length;
    int zone;
} canon_t;

extern canon_t canon;

void initCanon();
int getZone(int, int, int, int);

#endif