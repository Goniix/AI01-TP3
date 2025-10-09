#ifndef TP3_H
#define TP3_H

typedef struct s_processus
{
    int pid;                     // identifiant du processus
    int arrivee;                 // temps d'arrivee
    int duree;                   // duree d'execution
    struct s_processus *suivant; // pointeur vers le processus suivant
} t_processus;

t_processus *processus_init(int pid, int arrivee, int duree);
void processus_free(t_processus *p);
t_processus *processus_load(char *nom_fichier, int nb_processus);
void processus_print(t_processus *process);

typedef struct s_FIFO
{
    t_processus *first; // élément en tête de la queue
    t_processus *last;  // dernier élément de la queue
    int size;           // taille de la queue
} FIFO;

FIFO *fifo_init();
FIFO *fifo_init_from_process(t_processus *p);
void fifo_free(FIFO *file);
int fifo_vide(FIFO *file);
FIFO *fifo_enfiler(FIFO *file, t_processus *p);
t_processus *fifo_defiler(FIFO *file);

#endif