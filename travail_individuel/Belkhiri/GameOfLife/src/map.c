#include "map.h"

int MAPSIZE = 20;

int **map;

int ***mapList;

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

int **saveMapState()
{
    int **mapCopy = malloc(MAPSIZE * sizeof(int *));
    for (int i = 0; i < MAPSIZE; i++)
    {
        mapCopy[i] = malloc(MAPSIZE * sizeof(int));
    }
    for (int i = 0; i < MAPSIZE; i++)
    {
        for (int j = 0; j < MAPSIZE; j++)
        {
            mapCopy[i][j] = map[i][j];
        }
    }
    return mapCopy;
}

void addMapToList(int **map)
{
    int i = 0;
    while (mapList[i] != NULL)
    {
        i++;
    }
    mapList[i] = map;
}

void checkForCycle()
{
    // count number of maps saved
    int i = 0;
    while (mapList[i] != NULL)
    {
        i++;
    }

    int j = 0;
    for (j = 0; j < i; j++)
    {
        int cycle = 1;
        // compare values of maps
        int k = 0;
        for (k = 0; k < MAPSIZE; k++)
        {
            for (int l = 0; l < MAPSIZE; l++)
            {
                if (mapList[j][k][l] != map[k][l])
                {
                    cycle = 0;
                }
            }
        }
        if (cycle == 1)
        {
            printf("Cycle found!\n");
            break;
        }
    }
}