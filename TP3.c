#include <stdio.h>
#include <stdlib.h>
#include "TP3.h"

// Il veut juste que le rapport soit clait, 2 pages, on calcule la complexité, pas besoin de justifier les choix de fonction tant que ça marche
// faire le diagrame de gant si on a le temps, ça donne des points bonnus, sinon il veut juste la trace d'execution
t_processus *processus_init(int pid, int arrivee, int duree)
{
    t_processus *res = malloc(sizeof(t_processus));
    res->pid = pid;
    res->arrivee = arrivee;
    res->duree = duree;
    res->suivant = NULL;
    return res;
}

void processus_free(t_processus *p)
{
    if (p != NULL)
    {
        processus_free(p->suivant);
        free(p);
    }
}

t_processus *processus_load(char *nom_fichier, int nb_processus)
{
    FILE *file = fopen(nom_fichier, "r");
    t_processus *res = NULL;
    t_processus *last = NULL;
    for (int i = 0; i < nb_processus; i++)
    {
        char *res_str;
        long unsigned int res_str_size;
        getline(&res_str, &res_str_size, file); // lire avec getline
        int pid, arrive, duree;
        sscanf(res_str, "%d %d %d", &pid, &arrive, &duree);

        t_processus *new = processus_init(pid, arrive, duree);
        if (res == NULL)
        {
            res = new;
        }
        else
        {
            last->suivant = new;
        }
        last = new;
    }
    fclose(file);
    return res;
}

void processus_print(t_processus *process)
{
    if (process != NULL)
    {

        printf(" %d | %d | %d\n", process->pid, process->arrivee, process->duree);
        processus_print(process->suivant);
    }
}

FIFO *fifo_init()
{
    FIFO *res = malloc(sizeof(FIFO));
    res->first = NULL;
    res->last = NULL;
    res->size = 0;
    return res;
}

FIFO *fifo_init_from_process(t_processus *p)
{
    FIFO *res = fifo_init();
    t_processus *elem = p;
    while (elem != NULL)
    {
        fifo_enfiler(res, elem);
        elem = elem->suivant;
    }
    return res;
}

void fifo_free(FIFO *file)
{
    processus_free(file->first);
    free(file);
}

int fifo_vide(FIFO *file)
{
    return file->size == 0 ? 1 : 0;
}

FIFO *fifo_enfiler(FIFO *file, t_processus *p)
{
    if (fifo_vide(file))
    {
        file->first = p;
        file->last = p;
    }
    else
    {
        file->last->suivant = p;
        file->last = p;
    }
    file->size++;
}

int main()
{
    t_processus *open = processus_load("file", 3);
    FIFO *fif = fifo_init_from_process(open);
    processus_print(fif->first);
    printf("\n");
    processus_print(fif->last);
    printf("size: %d\n", fif->size);

    return 0;
}