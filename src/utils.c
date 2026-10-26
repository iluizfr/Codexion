# include "codexion.h"

/*Checks if the manager is stopped*/
int     is_stopped(t_manager *manager)
{
    int stopped;

    pthread_mutex_lock(&manager->mutex);
    stopped = manager->stop;
    pthread_mutex_unlock(&manager->mutex);
    return stopped;
}

/*Marks the flag to stop in the manager struct*/
void    stop_simulation(t_manager *manager)
{
    pthread_mutex_lock(&manager->mutex);
    manager->stop = 1;
    pthread_mutex_unlock(&manager->mutex);
}

/*refactor of the messages*/
void    log_message(t_manager *manager, long timestamp, int coder_id, const char *message, char *color)
{
    char  *null_color = "\33[0m";

    pthread_mutex_lock(&manager->log_mutex);
    printf("%s%ld %d %s%s\n", color, timestamp, coder_id, message, null_color);
    pthread_mutex_unlock(&manager->log_mutex);
}

/*checks if the coder needs to compile*/
int     need_to_compile(t_coder *coder, int compiles_required)
{
    if (coder->compile_count < compiles_required)
        return (1);
    return (0);
}

/*destroy the mutex used*/
void    clean(t_manager *manager)
{
    int i;

    i = 0;
    if (manager->data.number_of_coders == 1)
    {
        pthread_mutex_destroy(&manager->coders[0].left_dongle->mutex);
        pthread_mutex_destroy(&manager->coders[0].right_dongle->mutex);
    }
    else
    {
        while (i < manager->data.number_of_coders)
            pthread_mutex_destroy(&manager->dongles[i++].mutex);
    }
}