#include "gandiag.h"
#include <stdlib.h>
#include <stdio.h>

gd_timestep *init_timestep(int t, int *arrival, int arrival_count, int current_process)
{
    gd_timestep *res = malloc(sizeof(gd_timestep));
    res->t = t;
    res->current_process = current_process;
    res->arrivals = arrival;
    res->n_arrival = arrival_count;
    res->next = NULL;
    return res;
}

void add_timestep(gd_timestep *current, gd_timestep *new)
{
    current->next = new;
}

void free_timestep(gd_timestep **stepaddr)
{
    rescurs_free_timestep(*stepaddr);
    *stepaddr = NULL;
}

void rescurs_free_timestep(gd_timestep *step)
{
    if (step != NULL)
    {
        rescurs_free_timestep(step->next);
        free(step);
    }
}

void print_timestep(gd_timestep *step)
{
    gd_timestep *ptr = step;
    printf("\n| t |");
    while (ptr != NULL)
    {
        printf("%03d|", ptr->t);
        ptr = ptr->next;
    }

    ptr = step;
    printf("\n|prc|");
    while (ptr != NULL)
    {
        if (ptr->current_process == -1)
        {
            printf("   |");
        }
        else
        {
            printf("%03d|", ptr->current_process);
        }
        ptr = ptr->next;
    }
    ptr = step;
    printf("\narrivals :");
    while (ptr != NULL)
    {
        if (ptr->n_arrival > 0)
        {
            printf("\nt=%d:", ptr->t);
            for (int i = 0; i < ptr->n_arrival; i++)
            {
                printf("%03d", ptr->arrivals[i]);
            }
        }
    }
    printf("\n");
}
