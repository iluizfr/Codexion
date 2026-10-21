#include "codexion.h"

/*The refact action perfomed by the coder*/
void    refactor(t_coder *coder)
{
    log_message(coder->manager,
        elapsed_time(coder->manager),
        coder->id, "is refactoring");

    usleep(coder->manager->data.time_to_refactor * 1000);
}

/*The debug action performed by the coder*/
void    debug(t_coder *coder)
{
    log_message(coder->manager,
        elapsed_time(coder->manager),
        coder->id, "is debugging");

    usleep(coder->manager->data.time_to_debug * 1000);
    refactor(coder);
}

/*The compile action performed by the coder*/
int     compile(t_coder *coder, t_manager *manager)
{
    t_dongle    *first;
    t_dongle    *second;

    if (coder->left_dongle->dogle_id < coder->right_dongle->dogle_id)
    {
        first = coder->left_dongle;
        second = coder->right_dongle;
    }
    else
    {
        first = coder->right_dongle;
        second = coder->left_dongle;
    }

    pthread_mutex_lock(&first->mutex);
    log_message(manager, elapsed_time(manager), coder->id, "has taken a dongle");
    pthread_mutex_lock(&second->mutex);
    log_message(manager, elapsed_time(manager), coder->id, "has taken a dongle");

    pthread_mutex_lock(&manager->mutex);
    if (manager->stop)
    {
        pthread_mutex_unlock(&manager->mutex);
        pthread_mutex_unlock(&second->mutex);
        pthread_mutex_unlock(&first->mutex);
        return (0);
    }
    coder->last_compile = get_time();
    pthread_mutex_unlock(&manager->mutex);
    log_message(manager, elapsed_time(manager), coder->id, "is compiling");

    usleep(manager->data.time_to_compile * 1000);
    coder->compile_count++;
  
    pthread_mutex_unlock(&second->mutex);
    pthread_mutex_unlock(&first->mutex);

    debug(coder);

    return 1;
}