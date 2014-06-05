#include <stdio.h>
#include <stdlib.h>
#include "util.h"

int contient(struct cadre * un_cadre, int a_trouver, int nb_cadre)
{
	int i = 0;
	for(i = 0; i < nb_cadre; ++i)
	{
		if(un_cadre[i].page == a_trouver)
		{
			return i;
		}
	}
	return -1;
}

int indice_lointain(struct cadre * un_cadre,  struct ref_processus * ref, int nb_cadre, int indice_ref, int* temp)
{
    int indice = 0;
    int lointain = -1; //indice de la valeur de 'un_cadre' qui arrivera le plus lointain
    int j = indice_ref;
    int i = 0;
    int trouve = 0;//pour arreter la boucle au cas ou la valeur est trouve

    for(indice = 0; indice < nb_cadre; ++indice)
    {
        trouve = 0;
        for (i = j; i < ref->nbre_ref; ++i)
        {
            if (ref->references[i] == un_cadre[indice].page)
            {
                indice_ref = i;
                trouve = 1;
                break;
            }
        }
        if(trouve != 1)
        {
            *temp = indice;
            return -1;
        }
        if(lointain < indice_ref)
        {
            lointain = indice_ref;
        }
    }
    return lointain;
}

struct memoire_physique * algo_optimal( struct ref_processus * ref, int nb_cadre )
{
    struct memoire_physique * memo = (struct memoire_physique *)malloc(sizeof(struct memoire_physique));
    memo->algo = OPTIMAL;
    memo->cadres = (struct cadre *)malloc(sizeof(struct cadre) * nb_cadre);
    memo->nbre_cadres = nb_cadre;
    memo->nbre_defauts_pages = 0;
    int indice_ref = 0;

    for(indice_ref = 0; indice_ref < ref->nbre_ref; ++indice_ref)
    {
        int lointain = 0;
        int indice_cadre = 0;
        if (contient(memo->cadres, ref->references[indice_ref], nb_cadre) == -1)
        {
            int tmp = 0;
            //recherche de l'indice de la valeur de 'ref->references' le plus lointain qui sera reutilise
            lointain = indice_lointain(memo->cadres, ref, nb_cadre, indice_ref, &tmp);
            if(lointain != -1)
            {
                //recherche de l'indice de lointain dans le cadre
                indice_cadre = contient(memo->cadres, ref->references[lointain], nb_cadre);
                memo->cadres[indice_cadre].page = ref->references[indice_ref];
                memo->nbre_defauts_pages += 1;

            }else if(lointain == -1){

                memo->cadres[tmp].page = ref->references[indice_ref];
                memo->nbre_defauts_pages += 1;
            }
        }
    }
    return memo;
}

struct memoire_physique * algo_horloge( struct ref_processus * ref, int nb_cadre )
{
    struct memoire_physique * memo = (struct memoire_physique *)malloc(sizeof(struct memoire_physique));
    memo->algo = HORLOGE;
    memo->cadres = (struct cadre *)malloc(sizeof(struct cadre) * nb_cadre);
    memo->nbre_cadres = nb_cadre;
    memo->nbre_defauts_pages = 0;

    int indice_ref = 0;
    int bras = 0;
    int temp = 0;

    for(indice_ref = 0; indice_ref < ref->nbre_ref; ++indice_ref)
    {
        temp = contient(memo->cadres,ref->references[indice_ref], nb_cadre);
        if(temp == -1)
        {
            while(memo->cadres[bras].R == 1)
            {
                memo->cadres[bras].R = 0;
                bras = (bras + 1) % nb_cadre;
            }
            memo->cadres[bras].page = ref->references[indice_ref];
            memo->cadres[bras].R = 1;
            memo->nbre_defauts_pages += 1;
            bras = (bras + 1) % nb_cadre;
        }else{
            memo->cadres[temp].R = 1;
        }
    }
    return memo;
}

void mise_a_jour(struct cadre * un_cadre, int nb_cadre, int * cycle)
{
    int i = 0;
    for(i = 0; i < nb_cadre; ++i)
    {
        un_cadre[i].R = (unsigned char) un_cadre[i].R / 2;
    }
    *cycle = 0;
}


/**
*Methode qui retourne la position du cadre qui a le plus petit compteur de bit
*/
int plus_petit_compteur(struct cadre * un_cadre, int nb_cadre)
{
    unsigned char min = 0;
    int i = 0;
    int j = 0;
    min = (unsigned char) un_cadre[0].R;
    for(i = 1; i < nb_cadre; ++i)
    {
       if(min > (unsigned char) un_cadre[i].R )
       {
           min = (unsigned char) un_cadre[i].R;
           j = i;
       }
    }
    return j;
}

struct memoire_physique * algo_vieillissement( struct ref_processus * ref, int nb_cadre, int cycle )
{
    struct memoire_physique * memo = (struct memoire_physique *)malloc(sizeof(struct memoire_physique));
    memo->algo = VIEILLISSEMENT;
    memo->cadres = (struct cadre *)malloc(sizeof(struct cadre) * nb_cadre);
    memo->nbre_cadres = nb_cadre;
    memo->nbre_defauts_pages = 0;

    int indice_ref = 0;
    int compteur_cycle = 0;
    unsigned char n_bit = 128;

    int r = -1;//pour garder un indice au cas de referencement sans defaut de page
    int reference [nb_cadre];

    for(indice_ref = 0; indice_ref < ref->nbre_ref; ++indice_ref)
    {
        int indice_cadre = 0;
        int temp = 0;
        temp = contient(memo->cadres, ref->references[indice_ref], nb_cadre);
        if (temp == -1)
        {
            if(r != -1)
            {
                mise_a_jour(memo->cadres, nb_cadre, &compteur_cycle);
                for(r = 0; r < nb_cadre; ++r)
                {
                    if(reference[r] == 1)
                    {
                        memo->cadres[r].R = n_bit;
                    }
                    reference[r] = 0;
                }
                r = -1;
            }else{
                mise_a_jour(memo->cadres, nb_cadre, &compteur_cycle);
            }

            indice_cadre = plus_petit_compteur(memo->cadres, nb_cadre);
            memo->cadres[indice_cadre].page = ref->references[indice_ref];
            memo->cadres[indice_cadre].R = n_bit;
            memo->nbre_defauts_pages += 1;

        }else{
            r = temp;
            reference[r] = 1;
            ++compteur_cycle;
        }
        if(compteur_cycle >= cycle)
        {
            mise_a_jour(memo->cadres, nb_cadre, &compteur_cycle);
        }
    }

    return memo;
}
