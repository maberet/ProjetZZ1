#include "map.h"

int quelleTaillePourLesCases(int divise, int diviseur){
    int r;
    r = divise/diviseur;
    return r;
}

int nombrevoisin(int **grille,int i , int j,int w){
    int cpt=0; 

    if (grille[(w+i-1)%w][(w+j-1)%w]==1){cpt++;}
    if (grille[(i)%w][(w+j-1)%w]==1){cpt++;}
    if (grille[(i+1)%w][(w+j-1)%w]==1){cpt++;}
    if (grille[(i+1)%w][(j)%w]==1){cpt++;}
    if (grille[(w+i-1)%w][(j)%w]==1){cpt++;}
    if (grille[(w+i-1)%w][(j+1)%w]==1){cpt++;}
    if (grille[(i)%w][(j+1)%w]==1){cpt++;}
    if (grille[(w+i+1)%w][(j+1)%w]==1){cpt++;}
 
    return (cpt); 
}
void liberer_grille(int ** grille,int taille){
    int i= taille-1; 

    while(i>=0){
        free(grille[i]);
        i--;
    } 
    free(grille);

}

void init_grille( int **grille, int taille){
    int i;
    int j;
    printf("taille%d\n",taille);
    for (i=0;i<taille;i++){
        for (j=0;j<taille;j++){
            grille[i][j]=0;
        }
    }
}

void iteration_suivante(int **grille, int **nouvelle_grille ,int *regles_survie, int *regles_naissances, int taille){
    int i;
    int j; 
    int nb_voisin;

    for (i=0;i<taille;i++){
        for (j=0;j<taille; j ++){
            nb_voisin=nombrevoisin(grille,i,j,taille);
            if ((grille [i][j]==0)&& (regles_naissances[nb_voisin])){
                nouvelle_grille [i][j]=1;
            }
            else if  ((grille [i][j]==1)&& (!regles_survie[nb_voisin])){
                nouvelle_grille [i][j]=0;
            }
            else{nouvelle_grille[i][j]=grille[i][j];}
        }
    }

    grille=nouvelle_grille; 
}


void changer_case(int **grille,int i,int j,int taille){
    printf("%d\n",grille[i][j]);
    if (grille[i*taille][j/taille]==0){
        grille[i/taille][j/taille]=1;
        }
    else {
        grille[i/taille][j/taille]=0;
        }
}

