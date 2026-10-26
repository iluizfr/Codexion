#include "codexion.h"

/*monitor routine*/
void    *monitor_routine(void *arg)
{
    int         i;
    t_manager   *manager;
    int         burned_id;
    long        current_time;

    manager = (t_manager *)arg;
    while (!is_stopped(manager))
    {
        burned_id = 0;
        current_time = get_time();
        pthread_mutex_lock(&manager->mutex);
        i = 0;
        while (i < manager->data.number_of_coders)
        {
            if (!manager->coders[i].finished && current_time - manager->coders[i].last_compile >= manager->time_to_burnout)
            {
                burned_id = manager->coders[i].id;
                manager->stop = 1;
                break;
            }
            i++;
        }
        pthread_mutex_unlock(&manager->mutex);
        if (burned_id != 0)
        {
            log_message(manager, elapsed_time(manager), burned_id, "burned out", "\33[31m");
            return NULL;
        }
        usleep(1000);
    }
    return NULL;
}