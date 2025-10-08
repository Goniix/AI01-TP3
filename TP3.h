typedef struct s_processus
{
    int pid;                     // identifiant du processus
    int arrivee;                 // temps d'arrivee
    int duree;                   // duree d'execution
    struct s_processus *suivant; // pointeur vers le processus suivant
} t_processus;

t_processus *creer_processus(int pid, int arrivee, int duree);
void free_processus(t_processus *p);
t_processus *charger_processus(char *nom_fichier, int *nb_processus);