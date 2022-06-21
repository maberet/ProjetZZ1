#ifndef projet

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>

#endif 

/* ------------------------------- Déclaration des fonctions.---------------------------------------*/

int nombrevoisin(int **grille,int i , int j,int w);
void liberer_grille(int **grille,int taille);
void iteration_suivante(int **grille,int **nouvelle_grille, int *regles_mort, int *regles_naissances, int taille);
void changer_case(int **grille,int i,int j,int taille);
