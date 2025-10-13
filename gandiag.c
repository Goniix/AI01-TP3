#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gandiag.h"

gd *gdg_init(char *title)
{
    gd *res = malloc(sizeof(gd));
    res->section_list = NULL;
    res->section_count = 0;
    res->title = title;
    return res;
}

int gdg_section_index(gd *diag, char *section_name)
{
    for (int i = 0; i < diag->section_count; i++)
    {
        if (strcmp(diag->section_list[i].name, section_name) == 1)
            return i;
    }
    return -1;
}

int gdg_insert_section(gd *diag, char *section_name)
{
    if (gdg_section_index(diag, section_name) != -1)
        return 0;

    gd_section *sect = malloc(sizeof(gd_section));
    sect->name = section_name;
    return 1;
}

int gdg_insert_process(gd *diag, char *section_name, t_processus *process)
{
    int section_index = gdg_section_index(diag, section_name);
    if (section_index != -1)
    {
        char *mermaid_code;
        sprintf(mermaid_code, "PID: %d    : %d, %dy\n", process->pid, process->arrivee, process->duree);
        strcat(&diag->section_list[section_index], mermaid_code);
    }
    return 0;
}

void gdg_print_mermaid(gd *diag)
{
    printf("gantt\n");
    printf("    title %s\n");
    printf("    dateFormat X\n");
    printf("    axisFormat \%s\n");
    for (int i = 0; i < diag->section_count; i++)
    {
        gd_section section = diag->section_list[i];
        printf("\n");
        printf("    section %s\n", section.name);
        printf("    %s\n", section.content);
    }
}

void gdg_free(gd *diag)
{
    for (int i = 0; i < diag->section_count; i++)
    {
        free(&diag->section_list[i]);
    }
    free(diag);
}
