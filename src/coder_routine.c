#include "codexion.h"

/*coders routine*/
void	*coders_routine(void *arg)
{
    t_coder        *coder;
    t_manager      *manager;
     
    coder = (t_coder *)arg;
    manager = coder->manager;
    if (coder->manager->data.number_of_coders == 1)
    {
        pthread_mutex_lock(&coder->left_dongle->mutex);
        log_message(manager, elapsed_time(manager), coder->id, "has taken a dongle", coder->color);

        while (!is_stopped(manager))
            usleep(1000);
        pthread_mutex_unlock(&coder->left_dongle->mutex);
    }
    else
    {
        while (need_to_compile(coder, coder->compiles_required) && !is_stopped(manager))
        {
            compile(coder, manager);
            if (is_stopped(manager))
                break;
            debug(coder);
            if (is_stopped(manager))
                break;
            refactor(coder);
        }
    }
    pthread_mutex_lock(&manager->mutex);
    coder->finished = 1;
    pthread_mutex_unlock(&manager->mutex);
    return NULL;
}
