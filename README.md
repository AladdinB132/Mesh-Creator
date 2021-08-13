# Mesh-Creator
Créé un fichier .obj à partir de points ou de mesures d'un LIDAR

Ce projet a été créé dans le cadre d'un séjour de recherche participative dans le but de développer un capteur pour scanner des grottes en 3D.
Il a été commissioné par le CNRS et organisé par Objectif Science International.

## Crédits
- Arurikku
- Fidaty
- Caryo
- tweqx

## License
[GNUv3](https://www.gnu.org/licenses/gpl-3.0.fr.html)


# Guide d'utilisation


1. Démarrer "run.bat".

Démarrer le programme en tappant "meshCreator.exe" dans l'invite de commande

2. Différents paramètres sont à renseigner :


	- Le programme
	- Type du fichier d'entrée : -0 pour des coordonnées brut (x,y,z)
			   	      -1 pour un fichier du lidar (distance, angle, angle, qualité)
	- Fichier d'entrée (fichier que le programme doit traiter) 
	- Fichier de sortie (en .obj !) (emplacement du fichier qui sera créé à partir du fichier d'entrée)
	- Distance minimal entre les points à relier pour la modélisation.
	- Distance maximal entre les points à relier pour la modélisation.
	- Tout les combien de faces le programme doit-il faire part de l'avancement de la modélisation (ex: tappez 1000 pour un rappel toute les 1000 faces crées)



>Exemple à écrire après démarrage de run.bat : 
>
>`meshCreator.exe 0 C:\Users\user\path\input.obj C:\Users\user\path\output.obj 0 0.25 1000`
