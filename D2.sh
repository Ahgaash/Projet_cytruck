#!/bin/bash

temps_debut=$(date +%s)

cat data.csv|cut -d ';' -f5,6 | awk -F';' '{noms[$2] += $1} END {for (nom in noms) print noms[nom] ";" nom}' datak3.txt |sort -nr -t ';' -k1|head -10 > d2.txt


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
plot 'd2.txt' using 1:xtic(2) with boxes lc rgb "black" notitle 
EOF

convert graph_temp.png -rotate 90 graph_d2.png

rm graph_temp.png

temps_fin=$(date +%s)
temps_execution=$((temps_fin - temps_debut))
echo "Le temps d'exécution est de ${temps_execution} secondes."
xdg-open graph_d2.png




