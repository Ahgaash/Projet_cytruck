#!/bin/bash

# Extraction des colonnes 1 et 5 du fichier data.csv et sauvegarde dans s2.txt
cat data.csv | cut -d ';' -f1,5 > s2.txt

# Exécution du programme prog 
./prog

# Extraction des colonnes nécessaires à partir de resultat_s2.txt et sauvegarde dans temp1.txt
awk -F';' 'NR>0 {print $1,$2,$3,$4,$5,$6}' resultat_s2.txt > temp1.txt

# Utilisation de gnuplot pour générer un graphique des distances (min-max-moyenne) par trajet
gnuplot << EOF
set terminal pngcairo enhanced font 'arial,10' size 800,600
set output 'graph_S.png'

set title 'Distances Min-Max-Moyenne par Trajet'
set xlabel 'ID des Trajets'
set ylabel 'Distances en km'
set xtics rotate out

# Utilise filledcurve pour représenter la plage Min-Max en vert
# Utilise lines pour représenter la moyenne en rouge
plot 'temp1.txt' using 1:3:4 with filledcurve title 'Min-Max' lc rgb 'green', \
     'temp1.txt' using 1:5:xticlabels(2) with lines linecolor rgb "red" title 'Moy'
EOF

# Ouvre l'image générée avec le visualiseur d'image par défaut sur le système
xdg-open graph_S.png
