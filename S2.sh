#!/bin/bash

cat data.csv|cut -d ';' -f1,5 > s2.txt
./prog

awk -F';' 'NR>0 {print $1,$2,$3,$4,$5,$6}' resultat_s2.txt > temp1.txt


gnuplot << EOF
set terminal pngcairo enhanced font 'arial,10' size 800,600
set output 'distances_plot.png'


set title 'Distances Min-Max-Moyenne par Trajet'
set xlabel 'ID des Trajets'
set ylabel 'Distances en km'
set xtics rotate out
plot 'temp1.txt' using 1:3:4 with filledcurve title 'Min-Max' lc rgb 'green', \
     'temp1.txt' using 1:5:xticlabels(2) with lines linecolor rgb "red" title 'Moy'
EOF

xdg-open distances_plot.png
