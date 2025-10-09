#include <stdio.h>
#include <stdlib.h>
#include "TP3.h"

#define MAX_LINE_LENGTH 256
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

void processus_free(t_processus **process)
{
    // on demande un pointeur vers le process (qui est un pointeur en soit) pour pouvoir set le pointeur du process à NULL après free, et éviter des erreurs plus tard
    if (process && *process)
    {
        free(*process);
        *process = NULL; //
    }
}

void processus_free_recursive(t_processus *process)
{
    if (process)
    {
        processus_free_recursive(process->suivant);
        processus_free(&process);
    }
}

t_processus *processus_load(char *nom_fichier, int nb_process)
{
    FILE *file = fopen(nom_fichier, "r");
    t_processus *head = NULL;
    t_processus *last = NULL;
    for (int i = 0; i < nb_process; i++)
    {
        char line[MAX_LINE_LENGTH];      // une ligne ne doit pas faire plus de MAX_LINE_LENGTH char
        fgets(line, sizeof(line), file); // on lit la ligne brute
        int pid, arrive, duree;
        sscanf(line, "%d %d %d", &pid, &arrive, &duree); // extraction des valeurs

        t_processus *new = processus_init(pid, arrive, duree);
        if (!new)
        {
            // alloc failed
            fclose(file);
            return NULL;
        }
        if (head == NULL) // on regarde si on a déjà defini une tête (vrai uniquement à la première itération)
        {
            head = new;
        }
        else // sinon on update le last pour être la dernière ligne lue
        {
            last->suivant = new;
        }
        last = new;
    }
    fclose(file);
    return head;
}

void processus_print(t_processus *process)
{
    if (process)
    {

        printf(" %2d | %7d | %5d\n", process->pid, process->arrivee, process->duree);
        processus_print(process->suivant);
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

FIFO *fifo_init_from_process(t_processus *process)
{
    if (!process)
    {
        return NULL;
    }
    FIFO *res = fifo_init();
    t_processus *elem = process;
    while (elem != NULL)
    {
        fifo_enfiler(res, elem);
        elem = elem->suivant;
    }
    return res;
}

void fifo_free(FIFO **file)
{
    if (file && *file)
    {
        processus_free_recursive((*file)->first);
        free(*file);
        *file = NULL;
    }
    else
    {
        printf("!!! Trying to free NULL Fifo !!!\n");
    }
}

int fifo_vide(FIFO *file)
{
    if (!file)
    {
        printf("!!! Trying to size NULL Fifo !!!\n");
        return -1;
    }
    return file->size == 0 ? 1 : 0;
}

void *fifo_enfiler(FIFO *file, t_processus *process)
{
    int isEmpty = fifo_vide(file);
    if (isEmpty == -1)
    {
        printf("!!! Trying to queue on NULL Fifo !!!\n");
    }
    if (isEmpty == 1)
    {
        file->first = process;
        file->last = process;
    }
    else
    {
        file->last->suivant = process;
        file->last = process;
    }
    file->size++;
}

t_processus *fifo_defiler(FIFO *file)
{
    t_processus *res;
    if (fifo_vide(file) == 1)
    {
        printf("!!! Trying to unqueue from NULL Fifo !!!\n");
        return NULL;
    }
    else
    {
        res = file->first;
        file->first = file->first->suivant;
        file->size--;
        if (fifo_vide(file) == 1)
        {
            file->last = NULL;
        }
        return res;
    }
}

void fifo_print(FIFO *file)
{
    if (file)
    {

        printf("[==FIFO=(size : %d)==]\n", file->size);
        printf("pid | arrivee | duree\n");
        processus_print(file->first);
        printf("[====================]\n");
    }
    else
    {
        printf("!!! Trying to print NULL Fifo !!!\n");
    }
}

int main()
{
    t_processus *open = processus_load("file", 3);
    FIFO *fif = fifo_init_from_process(open);
    fifo_print(fif);

    return 0;
}