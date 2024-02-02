#!/bin/bash

# Enregistre le temps de début de l'exécution du script
temps_debut=$(date +%s)

# Utilise cat et awk pour extraire et sommer les distances parcourues par conducteur à partir des colonnes 5 et 6 du fichier data.csv
cat data.csv | cut -d ';' -f5,6 | awk -F';' '{noms[$2] += $1} END {for (nom in noms) print noms[nom] ";" nom}' > datak3.txt 

# Trie et sélectionne les 10 premiers résultats pour les conducteurs ayant parcouru la plus grande distance
cat datak3.txt | sort -nr -t ';' -k1 | head -10 > d2.txt

# Utilisation de gnuplot pour générer un diagramme en barres à partir des données extraites
gnuplot << EOF
set terminal pngcairo enhanced font "arial,10" size 800,600
set output 'graph_temp.png' 
set title "Top 10 Conducteurs par Distance parcourue"
set style data histogram
set style fill solid
set xlabel "Conducteurs"
set ylabel "Distance"
set ytics rotate by 90
set xtics rotate out
set boxwidth 0.5 
set grid x
set datafile separator ";" 

# Utilise une fonction pour récupérer les noms des conducteurs, utilise une couleur noire pour les barres
plot 'd2.txt' using 1:xtic(2) with boxes lc rgb "black" notitle 
EOF

# Effectue une rotation de l'image générée
convert graph_temp.png -rotate 90 graph_D2.png

# Supprime les fichiers temporaires générés
rm graph_temp.png
rm datak3.txt

# Enregistre le temps de fin de l'exécution du script
temps_fin=$(date +%s)

# Calcule et affiche le temps total d'exécution du script
temps_execution=$((temps_fin - temps_debut))
echo "Le temps d'exécution est de ${temps_execution} secondes."

# Ouvre l'image générée avec le visualiseur d'image par défaut sur le système
xdg-open graph_D2.png
