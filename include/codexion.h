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

typedef struct s_manager
{
    int         timestamp;
}               t_manager;

typedef struct	s_coder
{
    pthread_t		thread;
    int				id;
    int				compile_count;
    int             time;
    long			last_compile;

	t_dongle 		*left_dongle;
	t_dongle 		*right_dongle;
    t_manager       *manager;
    t_data			data;
}					t_coder;


void	*initialize_coders(t_data data);
int		parser(int argc, char **argv);
void	*routine(void *arg);
t_data	create_data(char **argv);

#endif