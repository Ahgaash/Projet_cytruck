#ifndef ARBRE_AVL_H
#define ARBRE_AVL_H

typedef struct info {
  int id;
  float max;
  float moy;
  float min;
  float etapes;
  float diff;
} info;

typedef struct Trajet {
  info info;
  struct Trajet *fg;
  struct Trajet *fd;
  int equilibre;
} Trajet;

Trajet *creertrajet(int id, float dist);
int hauteur_S(struct Trajet *trajet);
int max_S(int a, int b);
Trajet *rotationGauche_S(Trajet *y);
Trajet *rotationDroite_S(Trajet *x);
Trajet *equilibrage_S(Trajet *racine);
Trajet *insertionTrajet(Trajet *lst, Trajet *nv, int *h);
int moydiff(Trajet *racine);
Trajet *top50(Trajet *a, Trajet *tab, int taille, int *index);
Trajet *parcours_S(Trajet *racine, Trajet *tab, int *index);
int trierTableau(Trajet *tableau, int taille);
int inverserTableau(Trajet tableau[], int taille);

#endif /* ARBRE_AVL_H */

