#ifndef TP3_H
#define TP3_H

typedef struct s_processus
{
    int pid;                     // identifiant du processus
    int arrivee;                 // temps d'arrivee
    int duree;                   // duree d'execution
    struct s_processus *suivant; // pointeur vers le processus suivant
} t_processus;

typedef enum PROCESSFIELDS
{
    PID,
    ARRIVEE,
    DUREE
} PROCESSFIELDS;

/**
 * @brief Crée un process à partir des valeurs en paramètre (O de 1)
 *
 * @param pid PID du process
 * @param arrivee Instant t d'arrivée du process
 * @param duree Durée du processus
 * @return Une structure t_processus avec les données entrées
 */
t_processus *processus_init(int pid, int arrivee, int duree);

/**
 * @brief Détruit le processus passé en pramètre (O de 1)
 *
 * @param p Le processus
 */
void processus_free(t_processus **process);

/**
 * @brief Détruit le processus passé en pramètre, ansi que tous les processus suivants (O de n). Le pointeur vers le process est set à NULL après exécution
 * @param p Le processus
 */
void processus_free_recursive(t_processus *process);

/**
 * @brief Charge une liste de processus depuis un fichier (O de n). Les lignes ne doivent pas faire plus de MAX_LINE_LENGTH chars.
 *
 * @param nom_fichier Le chemin vers le fichier
 * @param nb_processes Nombre de processus à charger dans le fichier
 * @return Le processus de tête
 */
t_processus *processus_load(char *nom_fichier, int nb_processes);

/**
 * @brief Affiche un processus ligne par ligne dans le terminal (O de n)
 *
 * @param process La tête du processus à afficher
 */
void processus_print(t_processus *process);

// On opte pour une super structure, qui encapsule un process. ça permet de très efficament (au prix du cout mémoire de 2 pointeurs et un int en plus), d'avoir un cout d'enfilage et de défilage en O de 1
typedef struct s_FIFO
{
    t_processus *first; // élément en tête de la queue
    t_processus *last;  // dernier élément de la queue
    int size;           // taille de la queue
} FIFO;

/**
 * @brief Initialise une FIFO vide, avec ses pointeurs à NULL, et sa taille à 0
 *
 * @return Une FIFO vide
 */
FIFO *fifo_init();

/**
 * @brief Initialise une FIFO à partir d'un processus en insérant les process dans l'ordre process (les process ne sont pas modifiés)(O de n)
 * @param process La tête du processus
 *
 * @return Une FIFO peuplée des processus passés en paramètre
 */
FIFO *fifo_init_from_process(t_processus *process);

/**
 * @brief Initialise une FIFO à partir d'un processus, en triant les processus en fonction d'un field donné (l'ordre des process est modifié). (O de n**2)
 * @param process La tête du processus
 * @param field Le field par lequel trier la FIFO
 *
 * @return Une FIFO triée peuplée des processus passés en paramètre
 */
FIFO *fifo_init_sorted_from_process(t_processus *process, PROCESSFIELDS field);

/**
 * @brief Détruit les processus de la FIFO récursivement, puis la FIFO en elle même (O de n). La Fifo est set à null après exécution
 * @param queue Un pointeur vers le pointeur de la FIFO, pour qu'on puisse set le pointeur de la Fifo à NULL
 *
 */
void fifo_free(FIFO **queue);

/**
 * @brief Teste si la FIFO est vide (O de 1)
 * @param queue La FIFO
 *
 * @return 1 si elle ets vide, 0 sinon
 */
int fifo_is_empty(FIFO *queue);

/**
 * @brief Ajoute un élément à la fin de la FIFO (O de 1)
 * @param queue La FIFO
 * @param process Le processus à ajouter
 *
 */
void fifo_add(FIFO *queue, t_processus *process);

/**
 * @brief Ajoute un élément à la FIFO, de sorte à garder la FIFO triée. L'élément est inséré juste après les autres processus de même 'field' que lui. Si on enfile une fifo uniquement avec cette fonction alors elle est triée! (O de n)
 * @param queue La FIFO
 * @param process Le processus à ajouter (son 'suivant' est altéré!)
 * @param field Le field par lequel trier la FIFO
 *
 */
void fifo_add_sorted(FIFO *queue, t_processus *process, PROCESSFIELDS field);

/**
 * @brief Défile le premier élément de la FIFO (O de 1). La FIFO n'aura plus aucune trace du processus, et le processus suivant du processus dépilé est mis à NULL
 * @param queue La FIFO
 *
 * @return L'élément retiré (IL DOIT ÊTRE FREE MANUELLEMENT PLUS TARD)
 */
t_processus *fifo_unqueue(FIFO *queue);

/**
 * @brief Affiche le contenu de la FIFO depuis le processus de tête, le dernier processus ainsi que la taille (O de n).
 *
 * @param queue La FIFO à afficher
 */
void fifo_print(FIFO *queue);

/**
 * @brief Indique si la fifo est triée par un field particulier (O de n).
 *
 * @param queue La FIFO à tester
 * @param field Le field par lequel la FIFO devrait être triée
 * @return 1 si elle est triée par 'duree' dans l'ordre croissant, 0 sinon (si elle est vide ou NULL aussi)
 */
int fifo_is_sorted(FIFO *queue, PROCESSFIELDS field);

// les deux vont être pareils, on devra de tout de façon parse toute la liste à la recherche des process à t == arrivee
void simuler_fcfs(FIFO *tab, int nb_processes); // sauf que lui il est pas trié
void simuler_sjf(FIFO *tab, int nb_processes);  // et lui si

#endif