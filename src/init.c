#include "codexion.h"

/*initialize the data*/
t_data  init_data(char **argv)
{
    t_data  data;

    data.number_of_coders = atoi(argv[1]);
    data.time_to_burnout = atoi(argv[2]);
    data.time_to_compile = atoi(argv[3]);
    data.time_to_debug = atoi(argv[4]);
    data.time_to_refactor = atoi(argv[5]);
    data.number_of_compiles_required = atoi(argv[6]);
    data.dongle_cooldown = atoi(argv[7]);
    data.scheduler = argv[8];

    return data;
}

/*initialize dongles*/
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

/*initialize the coders data*/
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
