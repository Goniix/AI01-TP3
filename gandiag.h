#ifndef GANDIAG_H
#define GANDIAG_H

typedef struct s_gd_timestep
{
    int t;
    struct s_gd_arrival *arrivals;
    int current_process;

    struct s_gd_timestep *next;

} gd_timestep;

typedef struct s_gd_arrival
{
    int pid;
    struct s_gd_arrival *next;

} gd_arrival;

gd_timestep *init_timestep(int t, gd_arrival *arrival, int current_process);

void add_timestep(gd_timestep *current, gd_timestep *new);

void free_timestep(gd_timestep **stepaddr);

void print_timestep(gd_timestep *step);

gd_arrival *init_arrival(int arrival);
void add_arrival(gd_arrival *current, int new);
void free_arrival(gd_arrival **addrarrival);
#endif
