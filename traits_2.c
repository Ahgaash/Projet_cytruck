#include <stdio.h>
#include <stdlib.h>

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

Trajet *creertrajet(int id, float dist) {
  Trajet *nv = malloc(sizeof(Trajet));
  nv->info.id = id;
  nv->info.max = dist;
  nv->info.min = dist;
  nv->info.moy = dist;
  nv->info.etapes = 1;
  return nv;
}

int hauteur(struct Trajet *trajet) {
  if (trajet == NULL){
    return 0;}
  return trajet->equilibre;
}

int max(int a, int b) { return (a > b) ? a : b; }

Trajet *rotationGauche(Trajet *y) {
  Trajet *x = y->fd;
  Trajet *T2 = x->fg;

  x->fg = y;
  y->fd = T2;

  // Mise à jour des hauteurs
  y->equilibre = 1 + max(hauteur(y->fg), hauteur(y->fd));
  x->equilibre = 1 + max(hauteur(x->fg), hauteur(x->fd));

  return x;
}

Trajet *rotationDroite(Trajet *x) {
  Trajet *y = x->fg;
  Trajet *T2 = y->fd;

  y->fd = x;
  x->fg = T2;

  // Mise à jour des hauteurs
  x->equilibre = 1 + max(hauteur(x->fg), hauteur(x->fd));
  y->equilibre = 1 + max(hauteur(y->fg), hauteur(y->fd));

  return y;
}

Trajet *equilibrage(Trajet *racine) {
  int equilibre = hauteur(racine->fg) - hauteur(racine->fd);

  // Cas de déséquilibre à gauche
  if (equilibre > 1) {
    if (hauteur(racine->fg->fg) >= hauteur(racine->fg->fd)) {
      // Cas de déséquilibre à gauche-gauche
      return rotationDroite(racine);
    } else {
      // Cas de déséquilibre à gauche-droite
      racine->fg = rotationGauche(racine->fg);
      return rotationDroite(racine);
    }
  }

  // Cas de déséquilibre à droite
  if (equilibre < -1) {
    if (hauteur(racine->fd->fd) >= hauteur(racine->fd->fg)) {
      // Cas de déséquilibre à droite-droite
      return rotationGauche(racine);
    } else {
      // Cas de déséquilibre à droite-gauche
      racine->fd = rotationDroite(racine->fd);
      return rotationGauche(racine);
    }
  }

  return racine;
}

Trajet *insertionTrajet(Trajet *lst, Trajet *nv, int *h) {

  if (lst == NULL) {
    *h = 1;
    return nv;
  }

  if (nv->info.id < lst->info.id) {
    *h = -(*h);
    lst->fg = insertionTrajet(lst->fg, nv, h);
  } else if (nv->info.id > lst->info.id) {
    lst->fd = insertionTrajet(lst->fd, nv, h);
  } else if (nv->info.id == lst->info.id) {
    *h = 0;
    if(nv->info.max > lst->info.max){
      lst->info.max = nv->info.max;
    }
    if(nv->info.min < lst->info.min){
      lst->info.min = nv->info.min;
    }
    lst->info.moy = lst->info.moy + nv->info.max;
    lst->info.etapes ++;
    free(nv); // libérer le nœud nouvellement créé car il n'est pas utilisé
    return lst;
  }

  if (*h != 0) {
    lst->equilibre = lst->equilibre + *h;
    lst = equilibrage(lst);
    if (lst->equilibre == 0) {
      *h = 0;
    } else {
      *h = 1;
    }
  }

  return lst;
}

void moydiff(Trajet *racine) {
  if (racine != NULL) {
    moydiff(racine->fg);
    racine->info.moy = racine->info.moy/racine->info.etapes;
    racine->info.diff = racine->info.max - racine->info.min;
   // printf("id : %d  / max : %f / min : %f / moy : %f / diff : %f\n", racine->info.id,racine->info.max,racine->info.min,racine->info.moy,racine->info.diff);
    moydiff(racine->fd);
  }
}

Trajet *top50(Trajet *a, Trajet *tab, int taille, int *index) {
    if (*index < taille) {
        tab[*index] = *a;  
        (*index)++;
    } else {
        int minIndex = 0;
        for (int i = 1; i < taille; i++) {
            if (tab[i].info.diff < tab[minIndex].info.diff) {
                minIndex = i;
            }
        }

        if (a->info.diff > tab[minIndex].info.diff) {
            tab[minIndex] = *a; 
        }
    }
    return tab;
}


Trajet *parcours(Trajet *racine, Trajet *tab, int *index) {
    if (racine != NULL) {
        tab = parcours(racine->fg, tab, index);
        tab = top50(racine, tab, 50, index);
        tab = parcours(racine->fd, tab, index);
    }
    return tab;
    
}


void trierTableau(Trajet *tableau, int taille) {
    Trajet temp;
    int trie = 0;

    while (!trie) {
        trie = 1;
        for (int i = 0; i < taille - 1; i++) {
            if (tableau[i].info.diff > tableau[i + 1].info.diff) {
                // Échange des éléments si l'élément actuel est plus grand que l'élément suivant
                temp = tableau[i];
                tableau[i] = tableau[i + 1];
                tableau[i + 1] = temp;
                trie = 0; // Le tableau n'est pas encore trié
            }
        }
    }
}


void inverserTableau(Trajet tableau[], int taille) {
    int debut = 0;
    int fin = taille - 1;

    while (debut < fin) {
        // Échanger les éléments aux positions debut et fin
        Trajet temp = tableau[debut];
        tableau[debut] = tableau[fin];
        tableau[fin] = temp;

        // Déplacer les indices vers le centre du tableau
        debut++;
        fin--;
    }
}


int main(void) {

  Trajet *lst = NULL;
  int id;
  float dist;
  int hauteur = 0;
  
  FILE *fichier = fopen("s2.txt", "r");
  
  char ligneEnTetes[100]; // Vous pouvez ajuster la taille selon vos besoins
  if (fgets(ligneEnTetes, sizeof(ligneEnTetes), fichier) == NULL) {
  printf("Erreur de lecture de la première ligne du fichier.\n");
  fclose(fichier);
  return 1; // Quitter le programme avec code d'erreur
    }
  
  while (fscanf(fichier, "%d;%f", &id, &dist) == 2) {
    Trajet *nv = creertrajet(id,dist);
    lst = insertionTrajet(lst, nv, &hauteur);
  }
  
  fclose(fichier);
  moydiff(lst);
  
  Trajet *top50Array = malloc(sizeof(Trajet)*50);
  for(int i=0;i<50;i++){
            top50Array[i].info.id = -1;
            top50Array[i].equilibre = 0;
            top50Array[i].info.max = 0;
            top50Array[i].info.min = 0;
            top50Array[i].info.moy = 0;
            top50Array[i].info.diff = 0;
            top50Array[i].fg = NULL;
            top50Array[i].fd = NULL;
            
    
  }

  // Utilisation de la fonction parcours pour obtenir le top 10
  // Utilisation de la fonction parcours pour obtenir le top 10
  printf("\n\n");
  int index = 0;
  parcours(lst, top50Array,&index);

  trierTableau(top50Array,50);
    printf("\n\n");
  inverserTableau(top50Array,50);
  FILE *fichier_2 = fopen("resultat_s2.txt","w");
  for(int i = 0; i < 50; i++) {
   fprintf(fichier,"%d;%d;%f;%f;%f;%f\n",(i+1), top50Array[i].info.id,top50Array[i].info.max,top50Array[i].info.min,top50Array[i].info.moy,top50Array[i].info.diff);
}
  

    
  



  return 0;
}
