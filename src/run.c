#include "codexion.h"

/*principal function to run*/
void    run(t_data data)
{
    t_manager   manager;
    t_coder		coders[data.number_of_coders];
    t_dongle    dongles[data.number_of_coders];

    manager.stop = 0;
    manager.data = data;
    manager.n_compiles = 0;
    manager.coders = coders;
    manager.dongles = dongles;
    manager.start_time = get_time();
    pthread_mutex_init(&manager.mutex, NULL);
    pthread_mutex_init(&manager.log_mutex, NULL);
    manager.time_to_burnout = data.time_to_burnout;

    init_dongles(dongles, data.number_of_coders, data.dongle_cooldown);
    init_coders(coders, data.number_of_coders, dongles, data.number_of_compiles_required, &manager);

    manager_routine(&manager);
}
