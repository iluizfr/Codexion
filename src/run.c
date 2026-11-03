/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-frei <lde-frei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/02 17:19:31 by lde-frei          #+#    #+#             */
/*   Updated: 2026/11/03 15:42:38 by lde-frei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*principal function to run*/
void	run(t_data data)
{
	t_manager	manager;
	t_coder		*coders;
	t_dongle	*dongles;

	coders = malloc(sizeof(t_coder) * data.number_of_coders);
	dongles = malloc(sizeof(t_dongle) * data.number_of_coders);
	if (!coders || ! dongles)
		return ;
	manager.stop = 0;
	manager.data = data;
	manager.n_compiles = 0;
	manager.coders = coders;
	manager.dongles = dongles;
	manager.start_time = get_time();
	pthread_mutex_init(&manager.mutex, NULL);
	pthread_mutex_init(&manager.log_mutex, NULL);
	manager.time_to_burnout = data.time_to_burnout;
	init_dongles(dongles, data.number_of_coders, data.dongle_cooldown);
	init_coders(data.number_of_coders, dongles,
		data.number_of_compiles_required, &manager);
	manager_routine(&manager);
	free(coders);
	free(dongles);
}
