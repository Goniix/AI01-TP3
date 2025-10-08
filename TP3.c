#include <stdio.h>
#include <stdlib.h>
#include <TP3.h>

// Il veut juste que le rapport soit clait, 2 pages, on calcule la complexité, pas besoin de justifier les choix de fonction tant que ça marche
// faire le diagrame de gant si on a le temps, ça donne des points bonnus, sinon il veut juste la trace d'execution
t_processus *creer_processus(int pid, int arrivee, int duree)
{
    t_processus *res = malloc(sizeof(t_processus));
    res->pid = pid;
    res->arrivee = arrivee;
    res->duree = duree;
    res->suivant = NULL;
    return res;
}

void free_processus(t_processus *p)
{
    if (p != NULL)
    {
        free_processus(p->suivant);
        free(p);
    }
}

t_processus *charger_processus(char *nom_fichier, int *nb_processus)
{
    FILE *file = fopen(nom_fichier, "r");
    t_processus *res = NULL;
    t_processus *last = NULL;
    for (int i = 0; i < *nb_processus; i++)
    {
        // t_processus *next = creer_processus();
    }
    fclose(file);
}
