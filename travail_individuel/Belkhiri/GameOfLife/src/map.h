#ifndef _MAP_HEADER_
#define _MAP_HEADER_

#include "main.h"

extern int MAPSIZE;
extern int ** map;

void printMap();
void initMap();
void writeMap(char* filename);
long long int MapToBinary(int ** map);
void chargeNiveauFichier(char *, int **, int);

#endif