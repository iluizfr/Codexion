#ifndef CODEXION_H
# define CODEXION_H

# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <ctype.h>
# include <string.h>
# include <sys/time.h>

typedef struct s_manager t_manager;

typedef struct
{
    int	    number_of_coders;
    int		time_to_burnout;
    int		time_to_compile;
    int		time_to_debug;
    int		time_to_refactor;
    int		number_of_compiles_required;
    int     dongle_cooldown;
    char    *scheduler;
}		    t_data;

typedef struct
{
	int				dogle_id;
	int				dogle_cooldown;

    pthread_mutex_t	mutex;
}					t_dongle;

typedef struct	s_coder
{
    int				id;
    int             finished;
    long			last_compile;
    int				compile_count;
    int             compiles_required;

	t_dongle 		*right_dongle;
	t_dongle 		*left_dongle;
    t_manager       *manager;
    pthread_t		thread;

}					t_coder;

typedef struct s_manager
{
    int             i;
    int             stop;
    int             n_compiles;
    int             required_compiles;
    long            time_to_burnout;
    long            start_time;

    pthread_mutex_t log_mutex;
    pthread_mutex_t mutex;

    pthread_t       monitor_thread;
    pthread_t       thread;

    t_coder         *coders;
    t_dongle        *dongles;

    t_data          data;

}                   t_manager;


void	run(t_data data);
int		parser(int argc, char **argv);
t_data	create_data(char **argv);

void    *monitor_routine(void *arg);

int     simulation_stopped(t_manager *manager);
void    stop_simulation(t_manager *manager);
void    log_message(t_manager *manager, long timestamp, int coder_id, const char *message);

long    get_time(void);
long    elapsed_time(t_manager *manager);
int     has_burned(t_coder *coder, t_manager *manager);


#endif