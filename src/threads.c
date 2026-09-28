#include "codexion.h"


int get_dogles(t_coder *coder)
{
    
}


void    *routine(void *arg)
{
    t_coder *ptr = (t_coder *)arg;
    int time;

    time = 0;
    get_dogles(ptr);

    return NULL;
}

void *initialize_coders(t_data data)
{
    int         i;
    t_coder     coders[data.number_of_coders];

    i = 0;
    while (i < data.number_of_coders)
    {
        coders[i].id = i + 1;
        coders[i].compile_count = 0;
        coders[i].data = data;

        pthread_create(&coders[i].thread, NULL, routine, &coders[i]);
        i++;
    }

    i = 0;
    while (i < data.number_of_coders)
        pthread_join(coders[i++].thread, NULL);

    return NULL;
}

/*void *initialize(s_data data)
{
    return;
}*/