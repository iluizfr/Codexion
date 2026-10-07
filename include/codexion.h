#ifndef CODEXION_H
# define CODEXION_H

# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <ctype.h>
# include <string.h>

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
    int             time;
    int			    last_compile;
    int				compile_count;

	t_dongle 		*left_dongle;
	t_dongle 		*right_dongle;
    pthread_t		thread;
    t_data			data;

}					t_coder;

typedef struct s_manager
{
    int             timestamp;
    int             n_compiles;

    t_coder         *coders;
    pthread_t       thread;
    pthread_mutex_t mutex;

}                   t_manager;


void	*initialize(t_data data);
int		parser(int argc, char **argv);
void	*routine(void *arg);
t_data	create_data(char **argv);

#endif