#include "map.h"

int MAPSIZE = 20;

int **map;

int ***mapList;
int mapListSize = 0;

void allocateMap()
{
    map = malloc(MAPSIZE * sizeof(int *));
    for (int i = 0; i < MAPSIZE; i++)
    {
        map[i] = malloc(MAPSIZE * sizeof(int));
    }
}

void allocateMapList(int SIZE)
{
    mapList = malloc(SIZE * sizeof(int **));
}

void initMap()
{
    allocateMap();
    allocateMapList(100);
    for (int i = 0; i < MAPSIZE; i++)
    {
        for (int j = 0; j < MAPSIZE; j++)
        {
            map[i][j] = 0;
        }
    }
}

void printMap()
{
    for (int i = 0; i < MAPSIZE; i++)
    {
        for (int j = 0; j < MAPSIZE; j++)
        {
            printf("%d ", map[i][j]);
        }
        printf("\n");
    }
}

void writeMap(char *filename)
{
    FILE *f = fopen(filename, "w");
    for (int i = 0; i < MAPSIZE; i++)
    {
        for (int j = 0; j < MAPSIZE; j++)
        {
            fprintf(f, "%d ", map[i][j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

void addMapToList(int **map)
{
    mapList[mapListSize] = map;
    mapListSize++;
}