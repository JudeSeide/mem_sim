# Makefile du projet mem_sim
#

OPTIONS = -Wall -lpthread 

#Cibles construites sur l'appel de make
all : mem_sim

#Modele de fichier Makefile
#
#<cible>: <dependances>
#<tabulation><Commandes>
#
# Attention : les dépendances ne sont pas automatique. Vous devez ajouter les .h manuellement.
#

mem_sim: mem_sim.c algo.o util.o
	gcc -o mem_sim mem_sim.c algo.o util.o $(OPTIONS) 

algo.o: algo.c util.h
	gcc -c algo.c

util.o: util.c util.h
	gcc -c util.c

clean :
	rm -f *.o
	rm -f gestionnaire
	rm -f *~


