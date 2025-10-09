#ifndef TP3_H
#define TP3_H

typedef struct s_processus
{
    int pid;                     // identifiant du processus
    int arrivee;                 // temps d'arrivee
    int duree;                   // duree d'execution
    struct s_processus *suivant; // pointeur vers le processus suivant
} t_processus;

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
void processus_free(t_processus *p);
/**
 * @brief Détruit le processus passé en pramètre, ansi que tous les processus suivants (O de n)
 *
 * @param p Le processus
 */
void processus_free_recursive(t_processus *p);
/**
 * @brief Charge une liste de processus depuis un fichier (O de n)
 *
 * @param nom_fichier Le chemin vers le fichier
 * @param nb_processus Nombre de processus à charger dans le fichier
 * @return Le processus de tête
 */
t_processus *processus_load(char *nom_fichier, int nb_processus);
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
 * @brief Initialise une FIFO à partir d'un processus. L'ordre est conservé. (O de n)
 * @param p La tête du processus
 *
 * @return Une FIFO peuplée des processus passés en paramètre
 */
FIFO *fifo_init_from_process(t_processus *p);
/**
 * @brief Détruit les processus de la FIFO récursivement, puis la FIFO en elle même (O de n)
 * @param p La FIFO
 *
 */
void fifo_free(FIFO *file);
/**
 * @brief Teste si la FIFO est vide (O de 1)
 * @param file La FIFO
 *
 * @return 1 si elle ets vide, 0 sinon
 */
int fifo_vide(FIFO *file);
/**
 * @brief Ajoute un élément à la fin de la FIFO (O de 1)
 * @param file La FIFO
 * @param p Le processus à ajouter
 *
 */
void *fifo_enfiler(FIFO *file, t_processus *p);
/**
 * @brief Défile le premier élément de la FIFO (O de 1). La FIFO n'aura plus aucune trace du processus, et le processus suivant du processus dépilé est mis à NULL
 * @param file La FIFO
 *
 * @return L'élément retiré (IL DOIT ÊTRE FREE MANUELLEMENT PLUS TARD)
 */
t_processus *fifo_defiler(FIFO *file);

/**
 * @brief Affiche le contenu de la FIFO depuis le processus de tête, le dernier processus ainsi que la taille (O de n).
 *
 * @param process La file à afficher
 */
void fifo_print(FIFO *file);

#endif