/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-frei <lde-frei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/02 17:19:18 by lde-frei          #+#    #+#             */
/*   Updated: 2026/11/03 18:39:13 by lde-frei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	create_coders(t_manager *manager)
{
	int	i;
	int	created_coders;

	i = 0;
	created_coders = 0;
	while (i < manager->data.number_of_coders)
	{
		if (pthread_create(&manager->coders[i].thread, NULL,
				coder_routine, &manager->coders[i]) != 0)
		{
			stop_simulation(manager);
			break ;
		}
		created_coders++;
		i++;
	}
	return (created_coders);
}

void	analise(t_manager *manager)
{
	int	i;
	int	count;
	int	burned_id;

	i = 0;
	count = 0;
	burned_id = 0;
	pthread_mutex_lock(&manager->mutex);
	while (i < manager->data.number_of_coders)
	{
		if (manager->coders[i].finished)
			count++;
		if (manager->coders[i].burned)
			burned_id = manager->coders[i].id;
		i++;
	}
	printf("========== RESULTS ==========\n");
	printf("Coders finished: %d/%d\n", count, manager->data.number_of_coders);
	printf("Elapsed time: %ldms, %.1fs\n",
		get_time() - manager->start_time,
		(get_time() - manager->start_time) / 1000.0);
	if (burned_id)
		printf("Coder burned id: %d\n", burned_id);
	pthread_mutex_unlock(&manager->mutex);
}

/*manager routine*/
void	manager_routine(t_manager *manager)
{
	int	i;
	int	created_coders;

	i = 0;
	if (pthread_create(&manager->monitor_thread,
			NULL, monitor_routine, manager) != 0)
	{
		stop_simulation(manager);
		destroy_mutex(manager);
		return ;
	}
	created_coders = create_coders(manager);
	while (i < created_coders)
		pthread_join(manager->coders[i++].thread, NULL);
	analise(manager);
	stop_simulation(manager);
	pthread_join(manager->monitor_thread, NULL);
	destroy_mutex(manager);
}
