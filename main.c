#include <stdio.h>
#include <stdlib.h>
#include "TP3.c"

int main()
{
    t_processus *open = NULL;
    FIFO *fif = NULL;

    open = processus_load("processus2.txt", 5);
    fif = fifo_init_sorted_from_process(open, ARRIVEE);
    fifo_print(fif);
    printf("%d\n", fifo_is_sorted(fif, ARRIVEE));
    fifo_free(&fif);

    open = processus_load("processus2.txt", 5);
    fif = fifo_init_from_process(open);
    fifo_print(fif);
    simuler_fcfs(fif);

    fifo_free(&fif);

    return 0;
}