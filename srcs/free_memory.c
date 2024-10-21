#include "../inc/parsing.h"

int add_to_garbage(t_garbage **garbage, void *line)
{
    t_garbage *new;
    t_garbage *temp;

    new = malloc(sizeof(t_garbage));
    if (!new)
        return (perror(PROGRAM_NAME), 1);
    new->line = line;
    new->next = NULL;
    if (!*garbage)
    {
        *garbage = new;
        return (0);
    }
    temp = *garbage;
    while (temp->next)
        temp = temp->next;
    temp->next = new;
    return (0);
}

void free_garbage(t_garbage *garbage)
{
    t_garbage *temp;

    while (garbage)
    {
        temp = garbage;
        garbage = garbage->next;
        free(temp->line);
        free(temp);
    }
}


void free2d(char **str)
{
    int i;

    i = 0;
    while (str[i])
        free(str[i++]);
    free(str);
}


