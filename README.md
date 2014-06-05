Algorithmes de remplacements des pages et threads.

Taper _make_ dans un terminal pour builder le projet
Taper _make clean_ pour le nettoyer

=========
OBJECTIF:
=========

mem_sim -- Simuler les algorithmes de remplacements des pages

=========
SYNOPSIS:
=========

mem_sim nombre-de-cadres cycle fichier-sequence-references

============
DESCRIPTION:
============

* *nombre_de_cadres* : le nombre de cadre alloué au processus.

* _cycle_ : le nombre de références après lesquelles les compteurs
	des cadres sont mis à jour pour l’algorithme vieillissement
	si pas de défaut de pages.
* *fichier_sequence_references* : un simple fichier de texte contenant
	des entiers strictement positifs séparés
	par des caractères blancs (des espaces,
	des tabulations, des fins de ligne, etc).
	Le fichier comporte toujours au moins une
	ligne vide à la fin.
	
	Le programme mem_sim lit la séquence des numéros des pages référencées à partir du
	fichier fourni en paramètre et affiche pour chacun des algorithmes de remplacements
	des pages les informations suivantes :
	- le nom de l’algorithme et le nombre de défauts des pages
	- le contenu des cadres des pages à la fin ainsi que le champs du bit R (horloge)
	ou le compteur (vieillissement) (sauf pour l’algorithme optimal où seul le nombre
	de défauts des pages qui compte)

