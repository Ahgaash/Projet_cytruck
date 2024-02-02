#include <stdio.h>
#include <stdlib.h>
#include "traits.h"

// Ce programme est utilisé pour le traitement S

// Fonction pour créer un nouveau trajet (qui sera envoyé dans un AVL)
Trajet *creertrajet(int id, float dist) {
    Trajet *nv = malloc(sizeof(Trajet));
    nv->info.id = id;
    nv->info.max = dist;
    nv->info.min = dist;
    nv->info.moy = dist;
    nv->info.etapes = 1;
    return nv;
}

// Fonction pour obtenir la hauteur d'un nœud dans un arbre AVL
int hauteur_S(struct Trajet *trajet) {
    if (trajet == NULL) {
        return 0;
    }
    return trajet->equilibre;
}

// Fonction utilitaire pour obtenir le maximum de deux entiers
int max_S(int a, int b) { return (a > b) ? a : b; }

// Fonction de rotation gauche pour l'équilibrage de l'AVL
Trajet *rotationGauche_S(Trajet *y) {
    Trajet *x = y->fd;
    Trajet *T2 = x->fg;

    x->fg = y;
    y->fd = T2;

    // Mise à jour des hauteurs
    y->equilibre = 1 + max_S(hauteur_S(y->fg), hauteur_S(y->fd));
    x->equilibre = 1 + max_S(hauteur_S(x->fg), hauteur_S(x->fd));

    return x;
}

// Fonction de rotation droite pour l'équilibrage de l'AVL
Trajet *rotationDroite_S(Trajet *x) {
    Trajet *y = x->fg;
    Trajet *T2 = y->fd;

    y->fd = x;
    x->fg = T2;

    // Mise à jour des hauteurs
    x->equilibre = 1 + max_S(hauteur_S(x->fg), hauteur_S(x->fd));
    y->equilibre = 1 + max_S(hauteur_S(y->fg), hauteur_S(y->fd));

    return y;
}

// Fonction pour rééquilibrer un nœud d'AVL
Trajet *equilibrage_S(Trajet *racine) {
    int equilibre = hauteur_S(racine->fg) - hauteur_S(racine->fd);

    // Cas de déséquilibre à gauche
    if (equilibre > 1) {
        if (hauteur_S(racine->fg->fg) >= hauteur_S(racine->fg->fd)) {
            // Cas de déséquilibre à gauche-gauche
            return rotationDroite_S(racine);
        } else {
            // Cas de déséquilibre à gauche-droite
            racine->fg = rotationGauche_S(racine->fg);
            return rotationDroite_S(racine);
        }
    }

    // Cas de déséquilibre à droite
    if (equilibre < -1) {
        if (hauteur_S(racine->fd->fd) >= hauteur_S(racine->fd->fg)) {
            // Cas de déséquilibre à droite-droite
            return rotationGauche_S(racine);
        } else {
            // Cas de déséquilibre à droite-gauche
            racine->fd = rotationDroite_S(racine->fd);
            return rotationGauche_S(racine);
        }
    }

    return racine;
}

// Fonction récursive pour l'insertion d'un trajet dans l'AVL
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
        if (nv->info.max > lst->info.max) {
            lst->info.max = nv->info.max;
        }
        if (nv->info.min < lst->info.min) {
            lst->info.min = nv->info.min;
        }
        lst->info.moy = lst->info.moy + nv->info.max;
        lst->info.etapes++;
        free(nv); // libérer le nœud nouvellement créé car il n'est pas utilisé
        return lst;
    }

    if (*h != 0) {
        lst->equilibre = lst->equilibre + *h;
        lst = equilibrage_S(lst);
        if (lst->equilibre == 0) {
            *h = 0;
        } else {
            *h = 1;
        }
    }

    return lst;
}

// Fonction récursive pour calculer la moyenne et la différence de chaque trajet dans l'AVL
void moydiff(Trajet *racine) {
    if (racine != NULL) {
        moydiff(racine->fg);
        racine->info.moy = racine->info.moy / racine->info.etapes;
        racine->info.diff = racine->info.max - racine->info.min;
        moydiff(racine->fd);
    }
}

// Fonction pour sélectionner les 50 trajets avec les plus petites différences et les stocker dans un tableau
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

// Fonction récursive pour parcourir l'AVL et sélectionner les 50 trajets avec les plus petites différences
Trajet *parcours_S(Trajet *racine, Trajet *tab, int *index) {
    if (racine != NULL) {
        tab = parcours_S(racine->fg, tab, index);
        tab = top50(racine, tab, 50, index);
        tab = parcours_S(racine->fd, tab, index);
    }
    return tab;
}

// Fonction pour trier un tableau de trajets par différence
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

// Fonction pour inverser l'ordre des éléments d'un tableau
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

