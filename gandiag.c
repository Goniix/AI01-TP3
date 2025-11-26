#include "gandiag.h"
#include <stdlib.h>
#include <stdio.h>

gd_timestep *init_timestep(int t, gd_arrival *arrival, int current_process)
{
    gd_timestep *res = malloc(sizeof(gd_timestep));
    res->t = t;
    res->current_process = current_process;
    res->arrivals = arrival;
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
        printf("\nt=%d:", ptr->t);

        gd_arrival *arr_ptr = ptr->arrivals;
        while (arr_ptr != NULL)
        {
            printf("%03d", arr_ptr->pid);
            arr_ptr = arr_ptr->next;
        }
    }
    printf("\n");
}

gd_arrival *init_arrival(int arrival)
{
    gd_arrival *res = malloc(sizeof(gd_arrival));
    res->pid = arrival;
    res->next = NULL;
    return res;
}

void add_arrival(gd_arrival *current, int new)
{
    current->next = init_arrival(new);
}

void free_arrival(gd_arrival **addrarrival)
{
    recurs_free_arrival(*addrarrival);
    *addrarrival = NULL;
}

void recurs_free_arrival(gd_arrival *arrival)
{
    if (arrival != NULL)
    {
        recurs_free_arrival(arrival->next);
        free(arrival);
    }
}