#!/bin/bash

temps_debut=$(date +%s) 

cat data.csv|cut -d ';' -f1,5 > s2.txt
gcc -o prog32 traits_2.c
./prog32


awk -F';' 'NR>0 {print $1,$2,$3,$4,$5,$6}' resultat_s2.txt > temp1.txt


gnuplot << EOF
set terminal pngcairo enhanced font 'arial,10' size 800,600
set output 'graph_S.png'


set title 'Distances Min-Max-Moyenne par Trajet'
set xlabel 'ID des Trajets'
set ylabel 'Distances en km'
set xtics rotate out
plot 'temp1.txt' using 1:3:4 with filledcurve title 'Min-Max' lc rgb 'green', \
     'temp1.txt' using 1:5:xticlabels(2) with lines linecolor rgb "red" title 'Moy'
EOF

temps_fin=$(date +%s)
temps_execution=$((temps_fin - temps_debut))
echo "Le temps d'exécution est de ${temps_execution} secondes."
xdg-open graph_S.png
