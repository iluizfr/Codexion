#include "codexion.h"

t_data  create_data(char **argv)
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
