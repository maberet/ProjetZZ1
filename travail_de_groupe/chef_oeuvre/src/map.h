#ifndef MAP_HEADER_
#define MAP_HEADER_

#include "main.h"

#define MAPSIZE 20

#define TREE 1
#define WATER 2

extern int map[MAPSIZE][MAPSIZE];

void readMapFromFile(char * filename);
void updateMap();

#endif