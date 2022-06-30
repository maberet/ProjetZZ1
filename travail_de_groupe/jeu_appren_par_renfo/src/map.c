#include "map.h"

int map[MAP_HEIGHT][MAP_WIDTH];

void readMapFromFile(char *file_name)
{
    FILE *file = fopen(file_name, "r");
    if (file == NULL)
    {
        printf("Couldn't open map file.");
        exit(EXIT_FAILURE);
    }
    int i, j;
    for (i = 0; i < MAP_HEIGHT; i++)
    {
        for (j = 0; j < MAP_WIDTH; j++)
        {
            fscanf(file, "%d", &map[i][j]);
        }
    }
    fclose(file);
}

void printMap()
{
    int i, j;
    for (i = 0; i < MAP_HEIGHT; i++)
    {
        for (j = 0; j < MAP_WIDTH; j++)
        {
            printf("%d ", map[i][j]);
        }
        printf("\n");
    }
}