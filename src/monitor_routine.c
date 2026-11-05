/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-frei <lde-frei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/02 17:19:22 by lde-frei          #+#    #+#             */
/*   Updated: 2026/11/05 02:13:32 by lde-frei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	watch_for_burnout(t_manager *manager, int i, int burned_id, long ctime)
{
	while (!is_stopped(manager))
	{
		burned_id = 0;
		ctime = get_time();
		pthread_mutex_lock(&manager->mutex);
		i = 0;
		while (i < manager->data.number_of_coders)
		{
			if (!manager->coders[i].finished
				&& ctime - manager->coders[i].last_compile
				>= manager->time_to_burnout)
			{
				burned_id = manager->coders[i].id;
				manager->coders[i].burned = 1;
				manager->stop = 1;
				break ;
			}
			i++;
		}
		pthread_mutex_unlock(&manager->mutex);
		if (burned_id != 0)
		{
			log_message(manager, burned_id, "burned out", "\33[31m");
			pthread_mutex_lock(&manager->priority_queue.mutex);
			pthread_cond_broadcast(&manager->priority_queue.cond);
			pthread_mutex_unlock(&manager->priority_queue.mutex);
		}
		usleep(1000);
	}
}

/*monitor routine*/
void	*monitor_routine(void *arg)
{
	int			i;
	int			burned_id;
	long		current_time;
	t_manager	*manager;

	i = 0;
	burned_id = 0;
	current_time = get_time();
	manager = (t_manager *)arg;
	watch_for_burnout(manager, i, burned_id, current_time);
	return (NULL);
}
