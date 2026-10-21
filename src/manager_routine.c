#include "codexion.h"

/*manager routine*/
void    *manager_routine(void *arg)
{
    int         i;
    t_manager   *manager;

    i = 0;
    manager = (t_manager *)arg;
    if (pthread_create(&manager->monitor_thread, NULL, monitor_routine, manager) != 0)
    {
        stop_simulation(manager);
        return NULL;
    }

    while (i < manager->data.number_of_coders)
    {
        if (pthread_create(&manager->coders[i].thread, NULL, coders_routine, &manager->coders[i]) != 0)
        {
            stop_simulation(manager);
            break;
        }
        i++;
    }
    i = 0;
    while (i < manager->data.number_of_coders)
        pthread_join(manager->coders[i++].thread, NULL);

    stop_simulation(manager);
    pthread_join(manager->monitor_thread, NULL);
    clean(manager);
    return NULL;
}
