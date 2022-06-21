#ifndef MAP_HEADER_
#define MAP_HEADER_

#include "main.h"

#define MAPSIZE 20



extern int map[MAPSIZE][MAPSIZE];

void readMapFromFile(char * filename);

#endif