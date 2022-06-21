#ifndef _MAP_HEADER_
#define _MAP_HEADER_

#include "main.h"

extern int MAPSIZE;
extern int ** map;
extern int *** mapList;
extern int mapListSize;

void printMap();
void initMap();
void writeMap(char* filename);
void checkForCycle();
void addMapToList(int ** map);
int ** saveMapState();

#endif