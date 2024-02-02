#include <stdio.h>
#include "traitT.h"
#include "traits.h"


// Ce programme joint les code C du traitement S et T 

int main() {
    int choix;
    printf("\n\n");
    printf("Veuillez confirmer : ");
    printf("\n\n");
    printf("1. Traitement -T\n\n");
    printf("2. Traitement -S\n\n");

    do {
        printf("choix (1-2) : ");
        scanf("%d", &choix);
    } while (choix != 1 && choix != 2);

    if (choix == 1) {
        // Traitement -T
        Ville *racine = NULL;
        int h = 0;
        char ville[100];
        char ville2[100];
        int stepvilles;

        // Ouverture et traitement du fichier townA2.txt
        FILE *fichier = fopen("townA2.txt", "r");
        if (fichier == NULL) {
            perror("Erreur lors de l'ouverture du fichier");
            return 1; // Quitte le programme avec un code d'erreur
        }

        char ligneIgnore[256];
        if (fgets(ligneIgnore, sizeof(ligneIgnore), fichier) == NULL) {
            fprintf(stderr, "Erreur lors de la lecture de la première ligne\n");
            fclose(fichier);
            return 1;
        }

        while (fscanf(fichier, "%d;%99[^\n]\n", &stepvilles, ville) == 2) {
            Ville *nv = nouvelleVille(ville);
            racine = insertionVille(racine, nv, &h, stepvilles, 1);
        }

        fclose(fichier);

        // Ouverture et traitement du fichier townB2.txt
        FILE *fichier_ = fopen("townB2.txt", "r");
        if (fichier_ == NULL) {
            perror("Erreur lors de l'ouverture du fichier");
            return 1; // Quitte le programme avec un code d'erreur
        }

        char ligneIgnore2[256];
        if (fgets(ligneIgnore2, sizeof(ligneIgnore2), fichier_) == NULL) {
            fprintf(stderr, "Erreur lors de la lecture de la première ligne\n");
            fclose(fichier_);
            return 1;
        }

        while (fscanf(fichier_, "%d;%99[^\n]\n", &stepvilles, ville2) == 2) {
            Ville *nv = nouvelleVille(ville2);
            racine = insertionVille(racine, nv, &h, 0, 2);
        }

        fclose(fichier_);

        // Initialisation du tableau top10Array
        Ville *top10Array = malloc(10 * sizeof(Ville));
        for (int i = 0; i < 10; i++) {
            top10Array[i].equilibre = 0;
            top10Array[i].nombreTrajets = 0;
            strcpy(top10Array[i].nom, "rien");
            top10Array[i].fg = NULL;
            top10Array[i].fd = NULL;
        }

        printf("\n\n");
        int index = 0;
        parcours(racine, top10Array, &index);
        tri_insertion(top10Array, 10);

        printf("Arbre après insertion :\n");
        FILE *fichier_2 = fopen("resultat_T.txt", "w");
        for (int i = 0; i < 10; i++) {
            fprintf(fichier_2, "%s;%d;%d\n", top10Array[i].nom, top10Array[i].depart, top10Array[i].nombreTrajets);
        }
    } else {
        // Traitement -S
        Trajet *lst = NULL;
        int id;
        float dist;
        int hauteur = 0;

        // Ouverture et traitement du fichier s2.txt
        FILE *fichier = fopen("s2.txt", "r");

        char ligneEnTetes[100];
        if (fgets(ligneEnTetes, sizeof(ligneEnTetes), fichier) == NULL) {
            printf("Erreur de lecture de la première ligne du fichier.\n");
            fclose(fichier);
            return 1; // Quitter le programme avec code d'erreur
        }

        while (fscanf(fichier, "%d;%f", &id, &dist) == 2) {
            Trajet *nv = creertrajet(id, dist);
            lst = insertionTrajet(lst, nv, &hauteur);
        }

        fclose(fichier);
        moydiff(lst);

        // Initialisation du tableau top50Array
        Trajet *top50Array = malloc(sizeof(Trajet) * 50);
        for (int i = 0; i < 50; i++) {
            top50Array[i].info.id = -1;
            top50Array[i].equilibre = 0;
            top50Array[i].info.max = 0;
            top50Array[i].info.min = 0;
            top50Array[i].info.moy = 0;
            top50Array[i].info.diff = 0;
            top50Array[i].fg = NULL;
            top50Array[i].fd = NULL;
        }

        printf("\n\n");
        int index = 0;
        parcours_S(lst, top50Array, &index);

        trierTableau(top50Array, 50);
        printf("\n\n");
        inverserTableau(top50Array, 50);
        FILE *fichier_2 = fopen("resultat_s2.txt", "w");
        for (int i = 0; i < 50; i++) {
            fprintf(fichier, "%d;%d;%f;%f;%f;%f\n", (i + 1), top50Array[i].info.id, top50Array[i].info.max,
                    top50Array[i].info.min, top50Array[i].info.moy, top50Array[i].info.diff);
        }
    }

    return 0;
}

