#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "traitT.h"

#define MAX_LINE_LENGTH 256
#define MAX_VILLES 100


// Fonction pour créer une nouvelle ville
Ville *nouvelleVille(char nom[]) {
    Ville *ville = (Ville *)malloc(sizeof(Ville));
    strcpy(ville->nom, nom);
    ville->nombreTrajets = 1;
    ville->depart = 0; // Initialiser le nombre de départs à 0
    ville->fg = NULL;
    ville->fd = NULL;
    ville->equilibre = 0; // Nouveau nœud a une hauteur de 1
    return ville;
}

// Fonction pour obtenir la hauteur d'un nœud dans un arbre AVL
int hauteur(struct Ville *ville) {
    if (ville == NULL)
        return 0;
    return ville->equilibre;
}

// Fonction utilitaire pour obtenir le maximum de deux entiers
int max(int a, int b) { return (a > b) ? a : b; }

// Fonction pour effectuer une rotation simple à gauche
Ville *rotationGauche(Ville *y) {
    Ville *x = y->fd;
    Ville *T2 = x->fg;

    x->fg = y;
    y->fd = T2;

    // Mise à jour des hauteurs
    y->equilibre = 1 + max(hauteur(y->fg), hauteur(y->fd));
    x->equilibre = 1 + max(hauteur(x->fg), hauteur(x->fd));

    return x;
}

// Fonction pour effectuer une rotation simple à droite
Ville *rotationDroite(Ville *x) {
    Ville *y = x->fg;
    Ville *T2 = y->fd;

    y->fd = x;
    x->fg = T2;

    // Mise à jour des hauteurs
    x->equilibre = 1 + max(hauteur(x->fg), hauteur(x->fd));
    y->equilibre = 1 + max(hauteur(y->fg), hauteur(y->fd));

    return y;
}

// Fonction pour rééquilibrer un nœud d'AVL
Ville *equilibrage(Ville *racine) {
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

// Fonction récursive pour l'insertion d'une ville dans l'AVL
Ville *insertionVille(Ville *lst, Ville *nv, int *h, int c, int c1) {
    if (lst == NULL) {
        *h = 1;
        if (c == 1) {
            nv->depart++;
        }
        return nv;
    }
    if (strcmp(lst->nom, nv->nom) < 0) {
        *h = -(*h);
        lst->fg = insertionVille(lst->fg, nv, h, c, c1);
    } else if (strcmp(lst->nom, nv->nom) > 0) {
        lst->fd = insertionVille(lst->fd, nv, h, c, c1);
    } else if (strcmp(lst->nom, nv->nom) == 0) {
        *h = 0;
        if (c1 == 2) {
            lst->nombreTrajets++;
        }
        if (c == 1) {
            lst->depart++;
        }
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

// Parcours infixe de l'AVL pour afficher les informations des villes
void parcoursInfixe(Ville *racine) {

    if (racine != NULL) {
        parcoursInfixe(racine->fg);
        printf("%s nbr de fois : (%d)\n", racine->nom, racine->nombreTrajets);
        parcoursInfixe(racine->fd);
    }
}

// Fonction de tri par insertion pour trier un tableau de villes par nombre de trajets
void triInsertion(Ville *arr, int n) {

    int i, j;
    Ville cle;

    for (i = 1; i < n; i++) {
        cle = arr[i];
        j = i - 1;

        while (j >= 0 && arr[j].nombreTrajets > cle.nombreTrajets) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }

        arr[j + 1] = cle;
    }
}

// Fonction pour sélectionner les 10 villes avec le plus grand nombre de trajets et les stocker dans un tableau
Ville *top10(Ville *a, Ville *tab, int taille, int *index) {
    if (*index < taille) {
        tab[*index] = *a;
        (*index)++;
    } else {
        // Recherche de l'élément le plus petit dans le tableau
        int minIndex = 0;
        for (int i = 1; i < taille; i++) {
            if (tab[i].nombreTrajets < tab[minIndex].nombreTrajets) {
                minIndex = i;
            }
        }

        // Remplacement de l'élément le plus petit si nécessaire
        if (a->nombreTrajets > tab[minIndex].nombreTrajets) {
            tab[minIndex] = *a;
        }
    }
    return tab;
}

// Parcours de l'AVL pour sélectionner les 10 villes avec le plus grand nombre de trajets
Ville *parcours(Ville *racine, Ville *tab, int *index) {
    if (racine != NULL) {
        racine->nombreTrajets = racine->nombreTrajets + racine->depart;
        tab = parcours(racine->fg, tab, index);
        tab = top10(racine, tab, 10, index);
        tab = parcours(racine->fd, tab, index);
    }
    return tab;
}

// Fonction de tri par insertion pour trier un tableau de villes par nom
void tri_insertion(Ville *tableau, int taille) {
    int i, j;
    Ville cle;

    for (i = 1; i < taille; i++) {
        cle = tableau[i];
        j = i - 1;

        // Déplacer les éléments du tableau qui sont plus grands que la clé vers la droite
        while (j >= 0 && strcmp(tableau[j].nom, cle.nom) > 0) {
            tableau[j + 1] = tableau[j];
            j = j - 1;
        }

        // Insérer la clé à la bonne position
        tableau[j + 1] = cle;
    }
}

