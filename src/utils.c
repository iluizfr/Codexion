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
    int compile_count;

    pthread_mutex_lock(&coder->manager->mutex);
    compile_count = coder->compile_count;
    pthread_mutex_unlock(&coder->manager->mutex);
    return (compile_count < compiles_required);
}

/*destroy the mutex used*/
void    destroy_mutex(t_manager *manager)
{
    int i;

    i = 0;
    while (i < manager->data.number_of_coders)
    {
        pthread_mutex_destroy(&manager->dongles[i].mutex);
        i++;
    }
    pthread_mutex_destroy(&manager->mutex);
    pthread_mutex_destroy(&manager->log_mutex);
}