#!/bin/bash

# Enregistre le temps de début de l'exécution du script
temps_debut=$(date +%s)

# Utilise awk pour extraire et compter les occurrences des noms à partir de la colonne 6 du fichier data.csv
awk -F ';' 'NR>2 && !visited[$1]++ {count[$6]++} END {for (item in count) print count[item], item}' data.csv | sort -nr -t ';' -k1 | head -10 > datak1.txt

# Modifie le format de sortie du fichier datak1.txt pour qu'il soit compatible avec gnuplot
sed -i 's/\(.*\) \([^ ]*\) /\1;\2 /' datak1.txt

# Utilisation de gnuplot pour générer un diagramme en barres à partir des données extraites
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

# Utilise une fonction pour récupérer les noms, utilise une couleur rouge pour les barres
plot 'datak1.txt' using 1:xtic(2) with boxes lc rgb "red" notitle 

# Ajoute une marge entre les barres (10% de la largeur totale)
set style histogram cluster gap 1
EOF

# Effectue une rotation de l'image générée
convert bar_chart.png -rotate 90 graph_D1.png

# Supprime l'image temporaire générée
rm bar_chart.png

# Enregistre le temps de fin de l'exécution du script
temps_fin=$(date +%s)

# Calcule et affiche le temps total d'exécution du script
temps_execution=$((temps_fin - temps_debut))
echo "Le temps d'exécution est de ${temps_execution} secondes."

# Ouvre l'image générée avec le visualiseur d'image par défaut sur le système
xdg-open graph_D1.png

