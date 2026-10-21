#include "codexion.h"

 // ------ utils ------

long    get_time(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

long    elapsed_time(t_manager *manager)
{
    return (get_time() - manager->start_time);
}

int     has_burned(t_coder *coder, t_manager *manager)
{
    long    current_time;

    current_time = get_time();
    if (current_time - coder->last_compile >= manager->time_to_burnout)
        return (1);
    return (0);
}

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

// ------ initialization of dongles & coders ------

void    init_dongles(t_dongle *dongles, int n_coders, int cooldown)
{
    int i;

    i = 0;
    while (i < n_coders)
    {
        dongles[i].dogle_id = i + 1;
        dongles[i].dogle_cooldown = cooldown;
        pthread_mutex_init(&dongles[i].mutex, NULL);
        i++;
    }
}

void    init_coders(t_coder *coders, int n_coders, t_dongle *dongles, int compiles_required, t_manager *manager)
{
    int i;

    i = 0;
    while (i < n_coders)
    {
        if (i == 0)
            coders[i].left_dongle = &dongles[n_coders - 1];
        else
            coders[i].left_dongle = &dongles[i - 1];

        coders[i].finished = 0;
        coders[i].id = i + 1;
        coders[i].compile_count = 0;
        coders[i].right_dongle = &dongles[i];
        coders[i].compiles_required = compiles_required;
        coders[i].manager = manager;
        coders[i].last_compile = manager->start_time;
		i++;
    }
}

 // ------ hard work ------

int     need_to_compile(t_coder *coder, int compiles_required)
{
    if (coder->compile_count < compiles_required)
        return (1);
    return (0);
}

void    refactor(t_coder *coder)
{
    printf("%ld %d is refactoring\n", elapsed_time(coder->manager), coder->id);
    usleep(coder->manager->data.time_to_refactor * 1000);
}

void    debug(t_coder *coder)
{
    printf("%ld %d is debugging\n", elapsed_time(coder->manager), coder->id);
    usleep(coder->manager->data.time_to_debug * 1000);
    refactor(coder);
}

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
    log_message(manager, elapsed_time(manager), coder->id, "has taken a dongle\n");
    pthread_mutex_lock(&second->mutex);
    log_message(manager, elapsed_time(manager), coder->id, "has taken a dongle\n");

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

 // ------ routines ------

 void	*coders_routine(void *arg)
 {
     t_coder        *coder;
     t_manager      *manager;
     
    coder = (t_coder *)arg;
    manager = coder->manager;
    if (coder->manager->data.number_of_coders == 1)
    {
        pthread_mutex_lock(&coder->left_dongle->mutex);
        log_message(manager, elapsed_time(manager), coder->id, "has taken a dongle");

        while (!simulation_stopped(manager))
            usleep(1000);
        pthread_mutex_unlock(&coder->left_dongle->mutex);
    }
    else
    {
        while (need_to_compile(coder, coder->compiles_required) && !simulation_stopped(manager))
            compile(coder, manager);
    }
    pthread_mutex_lock(&manager->mutex);
    coder->finished = 1;
    pthread_mutex_unlock(&manager->mutex);
    return NULL;
}

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

 // ------ principal function ------

void    run(t_data data)
{
    t_coder		coders[data.number_of_coders];
    t_dongle    dongles[data.number_of_coders];
    t_manager   manager;

    manager.stop = 0;
    pthread_mutex_init(&manager.mutex, NULL);
    pthread_mutex_init(&manager.log_mutex, NULL);
    manager.data = data;
    manager.n_compiles = 0;
    manager.coders = coders;
    manager.start_time = get_time();
    manager.time_to_burnout = data.time_to_burnout;
    manager.dongles = dongles;

    init_dongles(dongles, data.number_of_coders, data.dongle_cooldown);
    init_coders(coders, data.number_of_coders, dongles, data.number_of_compiles_required, &manager);

    // Create the main thread wich will create more in the routine
    pthread_mutex_init(&manager.mutex, NULL);
    pthread_create(&manager.thread, NULL, manager_routine, &manager);
    pthread_join(manager.thread, NULL);
}
