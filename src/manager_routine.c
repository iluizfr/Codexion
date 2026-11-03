/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-frei <lde-frei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/02 17:19:18 by lde-frei          #+#    #+#             */
/*   Updated: 2026/11/03 15:24:42 by lde-frei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	create_coders(t_manager *manager)
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
	stop_simulation(manager);
	pthread_join(manager->monitor_thread, NULL);
	destroy_mutex(manager);
}
