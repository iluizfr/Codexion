#include "codexion.h"

/*The refact action perfomed by the coder*/
void    refactor(t_coder *coder)
{
    log_message(coder->manager,
        elapsed_time(coder->manager),
        coder->id, "is refactoring", coder->color);

    usleep(coder->manager->data.time_to_refactor * 1000);
}

/*The debug action performed by the coder*/
void    debug(t_coder *coder)
{
    log_message(coder->manager,
        elapsed_time(coder->manager),
        coder->id, "is debugging", coder->color);

    usleep(coder->manager->data.time_to_debug * 1000);
    refactor(coder);
}

/* Unlocks the dongle and updates the time of avaliability
   of the dongle.
*/
void release_dongle(t_dongle *dongle)
{
    dongle->avaliable_at = get_time() + dongle->dongle_cooldown;
    pthread_mutex_unlock(&dongle->mutex);
}

/* checks the order to acquire the dongle and waits the dongle
   cooldown.
*/
int acquire_dongles(t_coder *coder, t_dongle **first, t_dongle **second)
{
    if (coder->left_dongle->dongle_id < coder->right_dongle->dongle_id)
    {
        *first = coder->left_dongle;
        *second = coder->right_dongle;
    }
    else
    {
        *first = coder->right_dongle;
        *second = coder->left_dongle;
    }
    while (1)
    {
        pthread_mutex_lock(&(*first)->mutex);
        if (get_time() < (*first)->avaliable_at)
        {
            pthread_mutex_unlock(&(*first)->mutex);
            usleep(1000);
            continue ;
        }
        pthread_mutex_lock(&(*second)->mutex);
        if (get_time() < (*second)->avaliable_at)
        {
            pthread_mutex_unlock(&(*second)->mutex);
            pthread_mutex_unlock(&(*first)->mutex);
            usleep(1000);
            continue;
        }
        return (1);
    }
}

/*The compile action performed by the coder*/
int    compile(t_coder *coder, t_manager *manager)
{
    t_dongle    *first;
    t_dongle    *second;

    if (!acquire_dongles(coder, &first, &second))
        return (0);

    log_message(manager, elapsed_time(manager), coder->id, "has taken a dongle", coder->color);
    log_message(manager, elapsed_time(manager), coder->id, "has taken a dongle", coder->color);
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

    log_message(manager, elapsed_time(manager), coder->id, "is compiling", coder->color);
    usleep(manager->data.time_to_compile * 1000);
    coder->compile_count++;

    release_dongle(second);
    release_dongle(first);

    debug(coder);

    return 1;
}
