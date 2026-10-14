#include "codexion.h"

 // ------ codexion ------

long    get_time(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
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

 // ------ time management ------

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

void    init_coders(t_coder *coders, int n_coders, t_dongle *dongles, int compiles_required)
{
    int i;

    i = 0;
    while (i < n_coders)
    {
        if (i == 0)
            coders[i].left_dongle = &dongles[n_coders - 1];
        else
            coders[i].left_dongle = &dongles[i - 1];

        coders[i].id = i + 1;
        coders[i].compile_count = 0;
        coders[i].right_dongle = &dongles[i];
        coders[i].last_compile = 0;
        coders[i].compiles_required = compiles_required;
		i++;
    }
}

 // ------ initialization of pieces ------

int     need_to_compile(t_coder *coder, int compiles_required)
{
    if (coder->compile_count < compiles_required)
        return (1);

    return (0);
}

int     compile(t_coder *coder)
{
    pthread_mutex_lock(&coder->right_dongle->mutex);
    pthread_mutex_lock(&coder->left_dongle->mutex);
    coder->last_compile = get_time();

    printf("%ld %d is compiling\n", coder->last_compile, coder->id);

    coder->compile_count++;
  
    pthread_mutex_unlock(&coder->right_dongle->mutex);
    pthread_mutex_unlock(&coder->left_dongle->mutex);

    return 1;
}

 // ------ hard work ------

 void	*coders_routine(void *arg)
{

    t_coder     *coder;

    coder = (t_coder *)arg;

    while (need_to_compile(coder, coder->compiles_required))
    {
        compile(coder);
    }
    return NULL;
}

void    *manager_routine(void *arg)
{
    int         i;
    int         j;
    int         compiles;
    t_manager   *manager;
    t_coder     *coders;

    i = 0;
    j = 0;
    compiles = 0;
    manager = (t_manager *)arg;
    coders = &manager->coders;

    while (i < manager->data.number_of_coders)
    {
        pthread_create(coders[i].thread, NULL, coders_routine, &coders[i]);
        i++;
    }

    while (j < manager->data.number_of_coders)
    {
		pthread_join(coders[j++].thread, NULL);
    }

    return NULL;
}

 // ------ routines ------

void    run(t_data data)
{
    int			i;
    t_coder		coders[data.number_of_coders];
    t_dongle    dongles[data.number_of_coders];
    t_manager   manager;

	i = 0;
    manager.data = data;
    manager.n_compiles = 0;

    init_dongles(dongles, data.number_of_coders, data.dongle_cooldown);
    init_coders(coders, data.number_of_coders, dongles, data.number_of_compiles_required);

    // Create the main thread wich will create more in the routine
    pthread_mutex_init(&manager.mutex, NULL);
    manager.start_time = get_time();
    pthread_create(manager.thread, NULL, manager_routine, &manager);
    pthread_join(manager.thread, NULL);


    while (i < data.number_of_coders)
        pthread_mutex_destroy(&dongles[i++].mutex);

    pthread_mutex_destroy(&manager.thread);
}

 // ------ principal function ------