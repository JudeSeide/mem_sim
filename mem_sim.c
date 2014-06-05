#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>
#include "util.h"

int NOMBRE_DE_CADRES;
int CYCLE;
struct ref_processus INITIAL;

void *thread_optimal(void *arg)
{
   pthread_exit(algo_optimal(&INITIAL, NOMBRE_DE_CADRES));
}

void *thread_horloge(void *arg)
{
    pthread_exit(algo_horloge(&INITIAL, NOMBRE_DE_CADRES));
}

void *thread_vieillissement(void *arg)
{
    pthread_exit(algo_vieillissement(&INITIAL, NOMBRE_DE_CADRES,CYCLE));
}

int main(int argc, char *argv[])
{
    const int NB_ARGUMENT = 4;
    assert(argc == NB_ARGUMENT);

    NOMBRE_DE_CADRES = atoi(argv[1]);
    CYCLE = atoi(argv[2]);
    INITIAL = lireFichier(argv[3]);

    pthread_t optimal;
    pthread_t horloge;
    pthread_t vieillissement;

    if(pthread_create(&optimal, NULL, thread_optimal, NULL) != 0)
    {
        perror("pthread_create 1");
        return EXIT_FAILURE;
    }

    if(pthread_create(&horloge, NULL, thread_horloge, NULL) != 0)
    {
        perror("pthread_create 2");
        return EXIT_FAILURE;
    }

    if(pthread_create(&vieillissement, NULL, thread_vieillissement, NULL) != 0)
    {
        perror("pthread_create 3");
        return EXIT_FAILURE;
    }

    struct memoire_physique * resultat[3];
    if (pthread_join(optimal, (void*)&resultat[0])) {
        perror("pthread_join");
        return EXIT_FAILURE;
    }

    if (pthread_join(horloge, (void*)&resultat[1])) {
        perror("pthread_join");
        return EXIT_FAILURE;
    }

    if (pthread_join(vieillissement, (void*)&resultat[2])) {
        perror("pthread_join");
        return EXIT_FAILURE;
    }

    int i = 0;
    for(i = 0; i < 3; ++i)
    {
        print_memoire_physique(*resultat[i]);
    }

    return 0;
}
