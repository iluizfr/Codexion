/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-frei <lde-frei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/02 17:19:13 by lde-frei          #+#    #+#             */
/*   Updated: 2026/11/02 17:19:14 by lde-frei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*initialize the data*/
t_data	init_data(char **argv)
{
	t_data	data;

	data.number_of_coders = atoi(argv[1]);
	data.time_to_burnout = atoi(argv[2]);
	data.time_to_compile = atoi(argv[3]);
	data.time_to_debug = atoi(argv[4]);
	data.time_to_refactor = atoi(argv[5]);
	data.number_of_compiles_required = atoi(argv[6]);
	data.dongle_cooldown = atoi(argv[7]);
	data.scheduler = argv[8];
	return (data);
}

/*initialize dongles*/
void	init_dongles(t_dongle *dongles, int n_coders, int cooldown)
{
	int	i;

	i = 0;
	while (i < n_coders)
	{
		dongles[i].dongle_id = i + 1;
		dongles[i].dongle_cooldown = cooldown;
		dongles[i].avaliable_at = 0;
		pthread_mutex_init(&dongles[i].mutex, NULL);
		i++;
	}
}

/* Returns the color for the coder*/
char	*colors(int index)
{
	if (index == 0)
		return ("\33[32m");
	if (index == 1)
		return ("\33[33m");
	if (index == 2)
		return ("\33[34m");
	if (index == 3)
		return ("\33[35m");
	if (index == 4)
		return ("\33[36m");
	if (index == 5)
		return ("\33[37m");
	if (index == 6)
		return ("\33[38m");
	if (index == 7)
		return ("\33[39m");
	if (index == 8)
		return ("\33[40m");
	else
		return ("\33[32");
}

/*initialize the coders data*/
void	init_coders(int n_coders, t_dongle *dongles,
					int compiles_required, t_manager *manager)
{
	int	i;

	i = 0;
	while (i < n_coders)
	{
		if (i == 0)
			manager->coders[i].left_dongle = &dongles[n_coders - 1];
		else
			manager->coders[i].left_dongle = &dongles[i - 1];
		manager->coders[i].finished = 0;
		manager->coders[i].id = i + 1;
		manager->coders[i].compile_count = 0;
		manager->coders[i].right_dongle = &dongles[i];
		manager->coders[i].compiles_required = compiles_required;
		manager->coders[i].manager = manager;
		manager->coders[i].last_compile = manager->start_time;
		manager->coders[i].color = colors(i % n_coders);
		i++;
	}
}
