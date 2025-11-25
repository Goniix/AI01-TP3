#include "TP3.h"
#include "TP3.h"

#ifndef GANDIAG_H
#define GANDIAG_H

#define SECTION_MAX_SIZE 2000

typedef struct s_gd_timestep
{
    int t;
    int *arrivals;
    int n_arrival;
    int current_process;

    gd_timestep *next;

} gd_timestep;

gd_timestep *init_timestep(int t, int *arrival, int arrival_count, int current_process);

void add_timestep(gd_timestep *current, gd_timestep *new);

void free_timestep(gd_timestep **stepaddr);

void print_timestep(gd_timestep *step);
#endif
