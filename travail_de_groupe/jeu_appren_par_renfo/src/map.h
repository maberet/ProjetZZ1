#ifndef MAP_HEADER_
#define MAP_HEADER_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "main.h"

#define MAP_WIDTH 31
#define MAP_HEIGHT 11

extern int map[MAP_HEIGHT][MAP_WIDTH];

void readMapFromFile(char *file_name);
void printMap();

#endif