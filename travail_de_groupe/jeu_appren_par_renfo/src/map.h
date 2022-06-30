#ifndef MAP_HEADER_
#define MAP_HEADER_

#include "main.h"

#define MAP_WIDTH 31
#define MAP_HEIGHT 10

extern int map[MAP_HEIGHT][MAP_WIDTH];

void readMapFromFile(char *file_name);
void printMap();

#endif