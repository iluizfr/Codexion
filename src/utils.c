# include "codexion.h"

int simulation_stopped(t_manager *manager)
{
    int stopped;

    pthread_mutex_lock(&manager->mutex);
    stopped = manager->stop;
    pthread_mutex_unlock(&manager->mutex);
    return stopped;
}

void    stop_simulation(t_manager *manager)
{
    pthread_mutex_lock(&manager->mutex);
    manager->stop = 1;
    pthread_mutex_unlock(&manager->mutex);
}

void    log_message(t_manager *manager, long timestamp, int coder_id, const char *message)
{
    pthread_mutex_lock(&manager->log_mutex);
    printf("%ld %d %s\n", timestamp, coder_id, message);
    pthread_mutex_unlock(&manager->log_mutex);
}
