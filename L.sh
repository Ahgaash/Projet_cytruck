#!/bin/bash

temps_debut=$(date +%s)

cat data.csv | cut -d ';' -f1,5 | awk -F';' '{distances[$1] += $2} END {for (id in distances) printf "%s;%.3f\n", id, distances[id]}' | sort -nr -t ';' -k2 | head -10 | sort -nr -t ';' -k1 > lfinal.txt 





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
plot "lfinal.txt" using 2:xtic(1) with boxes notitle lc rgb "skyblue"
EOF

temps_fin=$(date +%s)
temps_execution=$((temps_fin - temps_debut))
echo "Le temps d'exécution est de ${temps_execution} secondes."
xdg-open graph_L.png

