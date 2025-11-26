#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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

t_processus *processus_load(char *nom_fichier, int *processes_loaded)
{
    FILE *file = fopen(nom_fichier, "r");
    t_processus *head = NULL;
    t_processus *last = NULL;
    (*processes_loaded) = 0;
    while (1)
    {
        char line[MAX_LINE_LENGTH];                                    // une ligne ne doit pas faire plus de MAX_LINE_LENGTH char
        char *ret = fgets(line, sizeof(char) * MAX_LINE_LENGTH, file); // on lit la ligne brute
        if (ret == NULL)
        {
            break; // EOF REACHED
        }
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
        (*processes_loaded)++;
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
        fifo_add(res, elem);
        elem = elem->suivant;
    }
    return res;
}

FIFO *fifo_init_sorted_from_process(t_processus *process, PROCESSFIELDS field)
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
        // on garde la référence vers le prochain element, car 'fifo_add_sorted' modifie les process
        next_elem = elem->suivant;
        fifo_add_sorted(res, elem, field, 0);
        elem = next_elem;
    }
    return res;
}

void fifo_free(FIFO **queue)
{
    // on vérifie la validité du pointeur vers le pointeur de la fifo, ainsi que la validité du poointeur de la fifo
    if (queue && *queue)
    {
        // on free les process de la fifo, la fifo et on détruit le pointeur
        processus_free_recursive((*queue)->first);
        free(*queue);
        *queue = NULL;
    }
    else
    {
        printf("!!! Trying to free NULL Fifo !!!\n");
    }
}

bool fifo_is_empty(FIFO *queue)
{
    if (!queue)
    {
        printf("!!! Trying to size NULL Fifo !!!\n");
        return -1;
    }
    return queue->size == 0;
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
        // si la fifo est vide, l'élément ajouté devient le premier et le dernier
        queue->first = process;
        queue->last = process;
    }
    else
    {
        // si elle contient déjà des éléments, on ajoute l'élément à la fin
        queue->last->suivant = process;
        queue->last = process;
    }
    queue->size++;
}

void fifo_add_sorted(FIFO *queue, t_processus *process, PROCESSFIELDS field, int ignore_first_n_index)
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

    // on ignore les n premiers éléments de la liste selon le param ignore_first_n_index
    while (curent_elem != NULL && ignore_first_n_index-- > 0)
    {
        previous_elem = curent_elem;
        curent_elem = curent_elem->suivant;
    }

    // on essaie de récupérer le process dans lequel on doit remplacer la référence vers le suivant, pour pouvoir insérer le process.
    // on garde en mémoire l'élément précédent, sur lequel on doit modifier le suivant pour pouvoir insérer le processus
    // la boucle suivante s'arrête quand la fin de la liste est atteinte ou le champ de l'élément actuel est plus grand que celui de l'élément qu'on essaie d'insérer
    switch (field)
    {
    case PID:
        while (curent_elem != NULL && curent_elem->pid <= process->pid)
        {
            previous_elem = curent_elem;
            curent_elem = curent_elem->suivant;
        }
        break;

    case ARRIVEE:
        while (curent_elem != NULL && curent_elem->arrivee <= process->arrivee)
        {
            previous_elem = curent_elem;
            curent_elem = curent_elem->suivant;
        }
        break;

    case DUREE:
        while (curent_elem != NULL && curent_elem->duree <= process->duree)
        {
            previous_elem = curent_elem;
            curent_elem = curent_elem->suivant;
        }
        break;

    default:
        printf("!!! Given field is unknown !!!\n");
        break;
    }

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
    t_processus *res;
    // on retourne le premier process
    res = queue->first;
    // on modifie les références pour l'oublier
    queue->first = queue->first->suivant;
    // on met à jour les infos secondaires de la liste
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
    }
    else
    {
        printf("!!! Trying to print NULL Fifo !!!\n");
    }
}

bool fifo_is_sorted(FIFO *queue, PROCESSFIELDS field)
{
    int isEmpty = fifo_is_empty(queue);
    if (isEmpty == -1)
    {
        printf("!!! Trying to test NULL Fifo !!!\n");
        return false;
    }
    if (isEmpty == 1)
    {
        printf("!!! Trying to test empty Fifo !!!\n");
        return false;
    }

    // on parse la liste à la recherche d'un élément dont le field donné est plus petit que celui des éléments précédents
    t_processus *elem = queue->first;
    int local_max = -1;
    switch (field)
    {
    case PID:
        while (elem != NULL)
        {

            if (elem->pid < local_max)
                return false;
            else
            {
                local_max = elem->pid;
                elem = elem->suivant;
            }
        }
        break;

    case ARRIVEE:
        while (elem != NULL)
        {

            if (elem->arrivee < local_max)
                return false;
            else
            {
                local_max = elem->arrivee;
                elem = elem->suivant;
            }
        }
        break;

    case DUREE:
        while (elem != NULL)
        {

            if (elem->duree < local_max)
                return false;
            else
            {
                local_max = elem->duree;
                elem = elem->suivant;
            }
        }
        break;

    default:
        printf("!!! Given field is unknown !!!\n");
        break;
    }

    return true;
}

void simuler_fcfs(FIFO *tab)
{
    // on trie tab par arrivée, donc il ne faut pas s'en resservir après
    FIFO *sortedArrival = fifo_init_sorted_from_process(tab->first, ARRIVEE);
    // la liste des éléments à traiter.
    // - le premier élément est l'élément en cours de traitement
    FIFO *ready = fifo_init();

    int t = 0;
    int remainingProcessTime = 0;

    printf("=== Simulation FCFS ===\n");
    while (fifo_is_empty(sortedArrival) == 0 || fifo_is_empty(ready) == 0)
    {
        // si un process est en cours, on l'avance
        if (remainingProcessTime > 0)
        {
            remainingProcessTime--;
        }
        // si le traitement du premier élément est terminé, le dépiler
        if (fifo_is_empty(ready) == 0 && remainingProcessTime == 0)
        {
            printf("t=%d : fin P%d\n", t, ready->first->pid);
            t_processus *to_free = fifo_unqueue(ready);
            processus_free(&to_free);
        }

        // on ajoute les process qui arrivent à l'instant t aux process en attente
        while (fifo_is_empty(sortedArrival) == 0 && sortedArrival->first->arrivee == t)
        {
            t_processus *arriving = fifo_unqueue(sortedArrival);
            fifo_add(ready, arriving);

            printf("t=%d : arrivee P%d (duree=%d)\n", t, arriving->pid, arriving->duree);
        }

        // on execute le prochain process si le systeme est libre
        if (fifo_is_empty(ready) == 0 && remainingProcessTime == 0)
        {

            remainingProcessTime = ready->first->duree;
            printf("t=%d : run P%d (duree=%d)\n", t, ready->first->pid, ready->first->duree);
        }

        t++;
    }
}

void simuler_sjf(FIFO *tab)
{
    // on trie tab par arrivée, donc il ne faut pas s'en resservir après
    FIFO *sortedArrival = fifo_init_sorted_from_process(tab->first, ARRIVEE);
    // la liste des éléments à traiter.
    // - le premier élément est l'élément en cours de traitement
    FIFO *ready = fifo_init();

    int t = 0;
    int remainingProcessTime = 0;

    printf("=== Simulation SJF ===\n");
    while (!fifo_is_empty(sortedArrival) || !fifo_is_empty(ready))
    {
        // si un process est en cours, on l'avance
        if (remainingProcessTime > 0)
        {
            remainingProcessTime--;
        }
        // si le traitement du premier élément est terminé, le dépiler
        if (fifo_is_empty(ready) == 0 && remainingProcessTime == 0)
        {
            printf("t=%d : fin P%d\n", t, ready->first->pid);
            t_processus *to_free = fifo_unqueue(ready);
            processus_free(&to_free);
        }

        // on ajoute les process qui arrivent à l'instant t aux process en attente, en les triant par durée ascendante
        while (!fifo_is_empty(sortedArrival) && sortedArrival->first->arrivee == t)
        {
            t_processus *arriving = fifo_unqueue(sortedArrival);

            int shouldIgnoreFirst = (!fifo_is_empty(ready) && remainingProcessTime != 0) ? 1 : 0;

            fifo_add_sorted(ready, arriving, DUREE, shouldIgnoreFirst);

            printf("t=%d : arrivee P%d (duree=%d)\n", t, arriving->pid, arriving->duree);
        }

        // on execute le prochain process si le systeme est libre
        if (!fifo_is_empty(ready) && remainingProcessTime == 0)
        {

            remainingProcessTime = ready->first->duree;
            printf("t=%d : run P%d (duree=%d)\n", t, ready->first->pid, ready->first->duree);
        }

        t++;
    }
}