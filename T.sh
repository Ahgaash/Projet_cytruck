#!/bin/bash

# Extraction des colonnes 2 et 3 du fichier data.csv et sauvegarde dans townA2.txt
cat data.csv | cut -d ';' -f2,3 > townA2.txt

# Extraction des colonnes 2 et 4 du fichier data.csv et sauvegarde dans townB2.txt
cat data.csv | cut -d ';' -f2,4 > townB2.txt

# Exécution du programme prog
./prog

# Utilisation de gnuplot pour générer un graphique des 10 villes les plus traversées
gnuplot << EOF
set terminal png
set output "graph_T.png"
set title "Les 10 villes les plus traversées"
set style data histogram
set style fill solid
set ylabel "NB ROUTES"
set xlabel "Noms des villes"
set xtic rotate by -45
set style histogram clustered
set datafile separator ';'

# Utilise deux colonnes pour représenter le nombre de routes depuis chaque ville
plot "resultat_T.txt" using 2:xtic(1) title "Première ville", \
     '' using 3:xtic(1) title "routes totales"
EOF

# Supprime les fichiers temporaires générés
rm townA2.txt
rm townB2.txt

# Ouvre l'image générée avec le visualiseur d'image par défaut sur le système
xdg-open graph_T.png
