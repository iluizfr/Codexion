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

typedef struct
{
    int		number_of_coders;
    int		time_to_burnout;
    int		time_to_compile;
    int		time_to_debug;
    int		time_to_refactor;
    int		number_of_compiles_required;
    int		dongle_cooldown;
    char	*scheduler;
}			t_data;

typedef struct
{
	int				dogle_id;
	int				dogle_cooldown;

    pthread_mutex_t	mutex;
}					t_dongle;

typedef struct	s_coder
{
    int				id;
    long			last_compile;
    int				compile_count;
    int             compiles_required;

	t_dongle 		*right_dongle;
	t_dongle 		*left_dongle;
    pthread_t		thread;

}					t_coder;

typedef struct s_manager
{
    int             n_compiles;
    int             required_compiles;
    long            time_to_burnout;
    long            start_time;

    t_coder         *coders;
    pthread_t       thread;
    pthread_mutex_t mutex;
    t_data          data;

}                   t_manager;


void	run(t_data data);
int		parser(int argc, char **argv);
void	*routine(void *arg);
t_data	create_data(char **argv);

#endif