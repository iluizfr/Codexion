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

// struct to store the data.
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

// dongles used by the coders.
typedef struct
{
	int				dongle_id;
	int				dongle_cooldown;
    long            avaliable_at;

    pthread_mutex_t	mutex;
}					t_dongle;

// coders (threads) who do the work.
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

// manager  struct wich have all coders, dongles and the monitor thread.
typedef struct s_manager
{
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

// ------ FUNCTIONS ------

// ------ run function ------
void	run(t_data data);

// ------ parser ------
int		parser(int argc, char **argv);

// ------ inits ------
void    init_coders(t_coder *coders, int n_coders, t_dongle *dongles, int compiles_required, t_manager *manager);
void    init_dongles(t_dongle *dongles, int n_coders, int cooldown);
t_data	init_data(char **argv);

// ------ routines ------
void    *monitor_routine(void *arg);
void    *manager_routine(void *arg);
void	*coders_routine(void *arg);

// ----- actions -----
int     compile(t_coder *coder, t_manager *manager);
void    refactor(t_coder *coder);
void    debug(t_coder *coder);

// ------ utils ------
void    log_message(t_manager *manager, long timestamp, int coder_id, const char *message);
int     need_to_compile(t_coder *coder, int compiles_required);
void    stop_simulation(t_manager *manager);
int     is_stopped(t_manager *manager);
void    clean(t_manager *manager);

// ------ time management functions ------
int     has_burned(t_coder *coder, t_manager *manager);
long    elapsed_time(t_manager *manager);
long    get_time(void);

#endif