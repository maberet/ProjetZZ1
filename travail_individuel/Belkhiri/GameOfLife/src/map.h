<<<<<<< HEAD
#ifndef _MAP_HEADER_
#define _MAP_HEADER_

#include "main.h"

#define MAPSIZE 20
extern int map[MAPSIZE][MAPSIZE];

void printMap(int map[MAPSIZE][MAPSIZE]);
void initMap(int map[MAPSIZE][MAPSIZE]);
void writeMap(int map[MAPSIZE][MAPSIZE], char* filename);

=======
#ifndef _MAP_HEADER_
#define _MAP_HEADER_

#include "main.h"

#define MAPSIZE 20
extern int map[MAPSIZE][MAPSIZE];

void printMap(int map[MAPSIZE][MAPSIZE]);
void initMap(int map[MAPSIZE][MAPSIZE]);

>>>>>>> 0a3e97f21219b823cdb1f8f8e7efee9633cdcc07
#endif