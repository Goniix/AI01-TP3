#include "TP3.h"

#ifndef GANDIAG_H
#define GANDIAG_H

#define SECTION_MAX_SIZE 2000

typedef struct s_gandiag
{
    char *title;
    gd_section *section_list;
    int section_count;

} gd;

typedef struct s_gd_section
{
    char *name;
    char content[SECTION_MAX_SIZE];

} gd_section;

/**
 * @brief Creates a Gantt diagram structure
 * @param title The diagram title
 * @returns Initialized Gantt diagram
 */
gd *gdg_init(char *title);

int gdg_section_index(gd *diag, char *section_name);

/**
 * @brief Inserts a new section into a Gantt diagram.
 * @param diag The diagram into which insert the section
 * @param section_name Name given to the newly created section. It is later used to insert processes into the section
 * @returns 1 if success, 0 if section already exists
 */
int gdg_insert_section(gd *diag, char *section_name);

/**
 * @brief Inserts a process into a Gantt diagram. Process data is append to 'content'.
 * @param diag The diagram into which insert the process.
 * @param section_name Name of the section the process will be inserted into.
 * @param process Inserted process.
 * @returns 1 if success, 0 if section is not found, -1 if section is full.
 */
int gdg_insert_process(gd *diag, char *section_name, t_processus *process);

/**
 * @brief Prints mermaid code of the diagram to the terminal
 * @param diag The printed diagram
 */
void gdg_print_mermaid(gd *diag);

/**
 * @brief Destroys a diagram
 * @param diag The destroyed diagram
 */
void gdg_free(gd *diag);

#endif
