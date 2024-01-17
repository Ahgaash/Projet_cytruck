#!/bin/bash


temps_debut=$(date +%s)

awk -F ';' 'NR>2 && !visited[$1]++ {count[$6]++} END {for (item in count) print count[item], item}' data.csv |sort -nr -t ';' -k1|head -10 > datak1.txt

sed -i 's/\(.*\) \([^ ]*\) /\1;\2 /' datak1.txt



gnuplot << EOF
set terminal pngcairo enhanced font "arial,10" size 800,600
set output 'bar_chart.png'
set title 'Top 10 des noms avec valeurs associées'
set xlabel 'Noms'
set ylabel 'Valeurs'
set style fill solid
set ytics rotate by 90
set xtics rotate out
set boxwidth 0.5 
set datafile separator ";" 
# Utilisez une fonction pour récupérer les noms
plot 'datak1.txt' using 1:xtic(2) with boxes lc rgb "black" notitle 

# Ajoutez une marge entre les barres (10% de la largeur totale)

set style histogram cluster gap 1
EOF

convert bar_chart.png -rotate 90 graph_D1.png
rm bar_chart.png
temps_fin=$(date +%s)
temps_execution=$((temps_fin - temps_debut))
echo "Le temps d'exécution est de ${temps_execution} secondes."
xdg-open graph_D1.png
