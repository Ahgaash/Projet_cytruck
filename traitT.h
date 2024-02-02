#ifndef VILLE_FUNCTIONS_H
#define VILLE_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "traitT.h"

#define MAX_LINE_LENGTH 256
#define MAX_VILLES 100

typedef struct Ville {
  char nom[50];
  int nombreTrajets;
  int depart;
  struct Ville *fg;
  struct Ville *fd;
  int equilibre;
} Ville;

Ville *nouvelleVille(char nom[]);
int hauteur(struct Ville *ville);
int max(int a, int b);
Ville *rotationGauche(Ville *y);
Ville *rotationDroite(Ville *x);
Ville *equilibrage(Ville *racine);
Ville *insertionVille(Ville *lst, Ville *nv, int *h, int c, int c1);
int parcoursInfixe(Ville *racine);
int triInsertion(Ville *arr, int n);
Ville *top10(Ville *a, Ville *tab, int taille, int *index);
Ville *parcours(Ville *racine, Ville *tab, int *index);
int tri_insertion(Ville *tableau, int taille);

#endif /* VILLE_FUNCTIONS_H */

