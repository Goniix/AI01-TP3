#include <stdio.h>
#include <stdlib.h>
#include "TP3.c"

int main()
{
    FIFO *queue = NULL; // tableau de processus
    int nb_processus = 0;
    int choix = 0;
    char nom_fichier[256];

    do
    {
        choix = 0;
        printf("\n===== Menu =====\n");
        printf("1. Charger processus depuis un fichier\n");
        printf("2. Simuler FCFS\n");
        printf("3. Simuler SJF\n");
        printf("4. Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix)
        {
        case 1: // Charger processus depuis un fichier
            printf("Nom du fichier : ");
            scanf("%s", nom_fichier);

            // Libération du tableau précédent si nécessaire
            if (queue != NULL)
            {
                fifo_free(&queue);
            }

            t_processus *tableau = NULL; // tableau de processus
            tableau = processus_load(nom_fichier, &nb_processus);
            queue = fifo_init_from_process(tableau);

            if (queue == NULL)
            {
                printf("Erreur lors du chargement du fichier.\n");
            }
            else
            {
                printf("Chargement réussi. %d processus chargés.\n", nb_processus);
            }
            break;

        case 2: // Simuler FCFS
            if (queue == NULL)
            {
                printf("Veuillez charger un fichier d'abord.\n");
            }
            else
            {
                gd_timestep *gantt = NULL;
                simuler_fcfs(queue, gantt);
                print_timestep(gantt);
                fifo_free(&queue);
            }
            break;

        case 3: // Simuler SJF
            if (queue == NULL)
            {
                printf("Veuillez charger un fichier d'abord.\n");
            }
            else
            {
                simuler_sjf(queue);
                fifo_free(&queue);
            }
            break;

        case 4: // Quitter
            printf("Au revoir !\n");
            break;

        default:
            printf("Choix invalide.\n");
        }

    } while (choix != 4);

    // Nettoyage mémoire avant sortie
    if (queue != NULL)
    {
        fifo_free(&queue);
    }

    return 0;
}

// int main()
// {
//     t_processus *open = NULL;
//     FIFO *fif = NULL;

//     int process_count;
//     open = processus_load("processus2.txt", &process_count);
//     fif = fifo_init_sorted_from_process(open, ARRIVEE);
//     fifo_print(fif);
//     printf("%d\n", fifo_is_sorted(fif, ARRIVEE));
//     fifo_free(&fif);

//     open = processus_load("file_simple", &process_count);
//     fif = fifo_init_from_process(open);
//     fifo_print(fif);
//     simuler_sjf(fif);

//     fifo_free(&fif);

//     return 0;
// }