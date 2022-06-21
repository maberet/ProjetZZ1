#ifndef _MAP_HEADER_
#define _MAP_HEADER_

#include "main.h"

#define MAPSIZE 20
extern int map[MAPSIZE][MAPSIZE];

void printMap(int map[MAPSIZE][MAPSIZE]);
void initMap(int map[MAPSIZE][MAPSIZE]);
void writeMap(int map[MAPSIZE][MAPSIZE], char* filename);

#endif