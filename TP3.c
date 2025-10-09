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

t_processus *processus_load(char *nom_fichier, int nb_processes)
{
    FILE *file = fopen(nom_fichier, "r");
    t_processus *head = NULL;
    t_processus *last = NULL;
    for (int i = 0; i < nb_processes; i++)
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
        // enfile l'élement dans la FIFO => modification du 'elem->suivant'
        fifo_queue(res, elem);
        elem = elem->suivant;
    }
    return res;
}

FIFO *fifo_init_sorted_from_process(t_processus *process)
{
    if (!process)
    {
        return NULL;
    }
    FIFO *res = fifo_init();
    t_processus *elem = process;
    t_processus *next_elem;

    while (elem != NULL)
    {
        // on garde la référence vers le prochain element, car 'fifo_enfiler_trie' modifie les process
        next_elem = elem->suivant;
        fifo_queue_sorted(res, elem);
        elem = next_elem;
    }
    return res;
}

void fifo_free(FIFO **queue)
{
    if (queue && *queue)
    {
        processus_free_recursive((*queue)->first);
        free(*queue);
        *queue = NULL;
    }
    else
    {
        printf("!!! Trying to free NULL Fifo !!!\n");
    }
}

int fifo_is_empty(FIFO *queue)
{
    if (!queue)
    {
        printf("!!! Trying to size NULL Fifo !!!\n");
        return -1;
    }
    return queue->size == 0 ? 1 : 0;
}

void fifo_add(FIFO *queue, t_processus *process)
{
    int isEmpty = fifo_is_empty(queue);
    if (isEmpty == -1)
    {
        printf("!!! Trying to queue on NULL Fifo !!!\n");
    }
    if (isEmpty == 1)
    {
        queue->first = process;
        queue->last = process;
    }
    else
    {
        queue->last->suivant = process;
        queue->last = process;
    }
    queue->size++;
}

void fifo_add_sorted(FIFO *queue, t_processus *process)
{
    int isEmpty = fifo_is_empty(queue);
    if (isEmpty == -1)
    {
        printf("!!! Trying to queue on NULL Fifo !!!\n");
        return;
    }

    t_processus *previous_elem = NULL;
    // si la liste est vide, il n'y a pas de suivant, et on laisse current_elem à NULL pour savoir qu'on doit mettre à jour 'last'
    t_processus *curent_elem = (isEmpty == 1) ? NULL : queue->first;
    // => donc si la liste était vide on se retrouve avec previous et current à NULL

    // on essaie de récupérer l'élement dans lequel on doit remplacer l'attribut suivant.
    // on garde en mémoire l'élément précédent, sur lequel on doit modifier le suivant pour pouvoir insérer le processus
    // la boucle suivante s'arrête quand la fin de la liste est atteinte ou la durée de l'élément actuel est plus grande que celle de l'élément qu'on essaie d'insérer
    while (curent_elem != NULL && curent_elem->duree <= process->duree)
    {
        previous_elem = curent_elem;
        curent_elem = curent_elem->suivant;
    }
    //

    // current_elem est NULL dans 2 cas:
    // - la fin de la liste est atteinte
    // - la liste était vide depuis le début
    // => on met à jour 'last', parceque dans les instructions suivantes on va insérer à la fin de la liste ('previous_elem' sera le dernier élément de la liste et 'current_elem' sera NULL)
    if (curent_elem == NULL)
    {
        queue->last = process;
    }

    // si previous_elem est NULL, ça veut dire qu'on n'a pas itéré une seule fois dans le while au dessus (le previous_elem est mis à jour à chaque itération), ce qui donne de cas:
    // - on a pas itéré parceque la liste était vide depuis le but
    // - on a pas itéré paceque la durée du premier élément de la liste est déjà plus grande que celle du processus
    // => on insère process en tête
    if (previous_elem == NULL)
    {
        process->suivant = queue->first;
        queue->first = process;
    }
    else
    {
        // si la condition précédente, on insère process après la liste après la tête :
        // - soit à la fin
        // - soit en plein millieu
        // => dans tout les cas le même comportement est attendu
        previous_elem->suivant = process;
        process->suivant = curent_elem;
    }

    queue->size++;
}

t_processus *fifo_unqueue(FIFO *queue)
{
    int isEmpty = fifo_is_empty(queue);
    t_processus *res;
    if (isEmpty == -1)
    {
        printf("!!! Trying to unqueue from NULL Fifo !!!\n");
        return NULL;
    }

    if (isEmpty)
    {
        printf("!!! Trying to unqueue from an empty Fifo !!!\n");
        return NULL;
    }

    res = queue->first;
    queue->first = queue->first->suivant;
    queue->size--;
    if (isEmpty)
    {
        queue->last = NULL;
    }
    return res;
}

void fifo_print(FIFO *queue)
{
    if (queue)
    {

        printf("[==FIFO=(size : %d)==]\n", queue->size);
        printf("pid | arrivee | duree\n");
        processus_print(queue->first);
        printf("[====================]\n");
        // processus_print(queue->last);
    }
    else
    {
        printf("!!! Trying to print NULL Fifo !!!\n");
    }
}

int fifo_is_sorted(FIFO *queue)
{
    t_processus *elem = queue->first;
    int local_max = -1;
    while (elem != NULL)
    {
        if (elem->duree < local_max)
            return 0;
        else
        {
            local_max = elem->duree;
            elem = elem->suivant;
        }
    }

    return 1;
}

int main()
{
    t_processus *open = NULL;
    FIFO *fif = NULL;

    open = processus_load("file", 6);
    fif = fifo_init_sorted_from_process(open);
    fifo_print(fif);
    printf("%d\n", fifo_is_sorted(fif));

    open = processus_load("file", 6);
    fif = fifo_init_from_process(open);
    fifo_print(fif);
    printf("%d\n", fifo_is_sorted(fif));

    return 0;
}