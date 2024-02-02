#!/bin/bash

# Enregistre le temps de début de l'exécution du script
temps_debut=$(date +%s)

# Utilise cat, cut et awk pour extraire et sommer les distances par route à partir des colonnes 1 et 5 du fichier data.csv
cat data.csv | cut -d ';' -f1,5 | awk -F';' '{distances[$1] += $2} END {for (id in distances) printf "%s;%.3f\n", id, distances[id]}' | sort -nr -t ';' -k2 | head -10 | sort -nr -t ';' -k1 > lfinal.txt 

# Utilisation de gnuplot pour générer un histogramme des distances par route
gnuplot << EOF
set terminal pngcairo enhanced font "arial,10" size 800,600
set output "graph_L.png"
set title "Histogramme des distances "
set ylabel "DISTANCE (km)"
set xlabel "ROUTE ID"
set style fill solid
set boxwidth 0.5 relative
set ytics nomirror
set yrange [0:*]
set xtics rotate by -45
set autoscale xfix
set datafile separator ";" 

# Utilise une couleur "skyblue" pour les barres
plot "lfinal.txt" using 2:xtic(1) with boxes notitle lc rgb "skyblue"
EOF

# Enregistre le temps de fin de l'exécution du script
temps_fin=$(date +%s)

# Calcule et affiche le temps total d'exécution du script
temps_execution=$((temps_fin - temps_debut))
echo "Le temps d'exécution est de ${temps_execution} secondes."

# Ouvre l'image générée avec le visualiseur d'image par défaut sur le système
xdg-open graph_L.png
