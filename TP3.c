#include <stdio.h>
#include <stdlib.h>
#include "TP3.h"

// Il veut juste que le rapport soit clait, 2 pages, on calcule la complexité, pas besoin de justifier les choix de fonction tant que ça marche
// faire le diagrame de gant si on a le temps, ça donne des points bonnus, sinon il veut juste la trace d'execution
t_processus *processus_init(int pid, int arrivee, int duree)
{
    t_processus *res = malloc(sizeof(t_processus));
    if (!res)
    {
        // alloc failed
        return NULL;
    }
    res->pid = pid;
    res->arrivee = arrivee;
    res->duree = duree;
    res->suivant = NULL;
    return res;
}

void processus_free(t_processus *p)
{
    free(p);
}

void processus_free_recursive(t_processus *p)
{
    if (p != NULL)
    {
        processus_free_recursive(p->suivant);
        processus_free(p);
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
        if (!new)
        {
            // alloc failed
            fclose(file);
            return NULL;
        }
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
    else
    {
        printf("\n");
    }
}

FIFO *fifo_init()
{
    FIFO *res = malloc(sizeof(FIFO));
    if (!res)
    {
        // alloc failed
        return NULL;
    }
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
    processus_free_recursive(file->first);
    free(file);
}

int fifo_vide(FIFO *file)
{
    return file->size == 0 ? 1 : 0;
}

void *fifo_enfiler(FIFO *file, t_processus *p)
{
    if (fifo_vide(file) == 1)
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

t_processus *fifo_defiler(FIFO *file)
{
    t_processus *res;
    if (fifo_vide(file) == 1)
    {
        return NULL;
    }
    else
    {
        res = file->first;
        file->first = file->first->suivant;
        file->size--;
        if (fifo_vide(file) == 0)
        {
            file->last == NULL;
        }
        return res;
    }
}

void fifo_print(FIFO *file)
{
    printf("content from 'first': \n");
    processus_print(file->first);
    printf("content from 'last': \n");
    processus_print(file->last);
    printf("size: %d\n", file->size);
}

int main()
{
    t_processus *open = processus_load("file", 3);
    FIFO *fif = fifo_init_from_process(open);
    fifo_print(fif);
    fifo_defiler(fif);
    fifo_print(fif);
    fifo_defiler(fif);
    fifo_print(fif);
    fifo_defiler(fif);
    fifo_print(fif);

    return 0;
}