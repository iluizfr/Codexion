/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-frei <lde-frei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/02 17:18:37 by lde-frei          #+#    #+#             */
/*   Updated: 2026/11/05 02:23:54 by lde-frei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# include <limits.h>

typedef struct s_manager	t_manager;
typedef struct s_coder		t_coder;
// struct to store the data.
typedef struct s_data
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
// dongles used by the coders.
typedef struct	s_dongle
{
	int				dongle_id;
	int				dongle_cooldown;
	long			avaliable_at;

	pthread_mutex_t	mutex;
}					t_dongle;
// coders (threads) who do the work.
struct	s_coder
{
	int				id;
	char			*color;
	int				burned;
	int				finished;
	long			last_compile;
	int				compile_count;
	int				compiles_required;

	t_dongle		*right_dongle;
	t_dongle		*left_dongle;
	t_manager		*manager;
	pthread_t		thread;
};

typedef struct	s_request
{
	t_coder			*coder;
	long			deadline;
	unsigned long	order;
}	t_request;

typedef struct s_priority_queue
{
	t_request		*heap;
	int				size;
	int				capacity;
	unsigned long	next_order;
	int				edf;
	t_coder			*acquiring;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
}	t_priority_queue;
// manager  struct wich have all coders, dongles and the monitor thread.
struct	s_manager
{
	int					stop;
	int					n_compiles;
	int					required_compiles;
	long				time_to_burnout;
	long				start_time;
	pthread_mutex_t		log_mutex;
	pthread_mutex_t		mutex;
	t_priority_queue	priority_queue;
	pthread_t			monitor_thread;
	t_coder				*coders;
	t_dongle			*dongles;
	t_data				data;

};
// ------ FUNCTIONS ------

// ------ run function ------
void	run(t_data data);
// ------ parser ------
int		parser(int argc, char **argv);
// ------ inits ------
void	init_coders(int n_coder, t_dongle *dongles,
			int ncomp, t_manager *manager);
void	init_dongles(t_dongle *dongles, int n_coders, int cooldown);
t_data	init_data(char **argv);
// ------ routines ------
void	*monitor_routine(void *arg);
void	manager_routine(t_manager *manager);
void	*coder_routine(void *arg);
// ------ priority queue ------
int		init_priority_queue(t_priority_queue *queue,
			int capacity, char *scheduler);
void	destroy_priority_queue(t_priority_queue *queue);
int		enqueue(t_priority_queue *queue, t_coder *coder, long deadline);
t_coder	*dequeue(t_priority_queue *queue);
int		remove_from_queue(t_priority_queue *queue, t_coder *coder);
// ----- actions -----
int		compile(t_coder *coder, t_manager *manager);
void	refactor(t_coder *coder);
void	debug(t_coder *coder);
void	release_dongle(t_dongle *dongle, t_manager *manager);
// ------ actions utils ------
int		wait_dongles(t_coder *coder, t_dongle **first, t_dongle **second);
// ------ utils ------
void	log_message(t_manager *manager, int coder_id,
			const char *message, char *color);
int		need_to_compile(t_coder *coder, int compiles_required);
void	stop_simulation(t_manager *manager);
int		is_stopped(t_manager *manager);
void	destroy_mutex(t_manager *manager);
// ------ time management functions ------
int		has_burned(t_coder *coder, t_manager *manager);
long	elapsed_time(t_manager *manager);
long	get_time(void);
#endif