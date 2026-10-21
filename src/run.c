#include "codexion.h"

/*principal function to run*/
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
