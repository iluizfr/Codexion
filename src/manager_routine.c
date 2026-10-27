#include "codexion.h"

/*manager routine*/
void    manager_routine(t_manager *manager)
{
    int i;
    int created_coders;

    i = 0;
    created_coders = 0;
    if (pthread_create(&manager->monitor_thread, NULL, monitor_routine, manager) != 0)
    {
        stop_simulation(manager);
        destroy_mutex(manager);
        return ;
    }

    while (i < manager->data.number_of_coders)
    {
        if (pthread_create(&manager->coders[i].thread, NULL,
                coder_routine, &manager->coders[i]) != 0)
        {
            stop_simulation(manager);
            break;
        }
        created_coders++;
        i++;
    }
    i = 0;
    while (i < created_coders)
        pthread_join(manager->coders[i++].thread, NULL);

    stop_simulation(manager);
    pthread_join(manager->monitor_thread, NULL);
    destroy_mutex(manager);
}
