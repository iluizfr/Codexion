/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-frei <lde-frei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/02 17:19:08 by lde-frei          #+#    #+#             */
/*   Updated: 2026/11/02 17:19:09 by lde-frei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
/*
int     is_on_list(int id, t_coder **list, int n_coders);

void remove_from_queue(t_manager *manager, t_coder *coder)
{
	int i;
	int j;

	pthread_mutex_lock(&manager->mutex);
	i = 0;
	while (i < manager->data.number_of_coders
		&& manager->priority_queue[i] != coder)
		i++;
	if (i < manager->data.number_of_coders)
	{
		j = i;
		while (j < manager->data.number_of_coders - 1)
		{
			manager->priority_queue[j] = manager->priority_queue[j + 1];
			j++;
		}
		manager->priority_queue[j] = NULL;
	}
	pthread_mutex_unlock(&manager->mutex);
}

int     is_first_in_queue(t_coder *coder, t_manager *manager)
{
	int result;

	pthread_mutex_lock(&manager->mutex);
	result = (manager->priority_queue[0] == coder);
	pthread_mutex_unlock(&manager->mutex);
	return (result);
}

static t_coder *get_minimun_locked(t_manager *manager, int queue_size)
{
	int     i;
	long    minimum;
	long    left_time;
	t_coder *selected;

	i = 0;
	minimum = LONG_MAX;
	selected = NULL;
	while (i < manager->data.number_of_coders)
	{
		if (!manager->coders[i].finished
			&& manager->coders[i].compile_count < manager->required_compiles
			&& !is_on_list(manager->coders[i].id,
				manager->priority_queue, queue_size))
		{
			left_time = manager->coders[i].last_compile
				+ manager->time_to_burnout - get_time();
			if (left_time < minimum)
			{
				minimum = left_time;
				selected = &manager->coders[i];
			}
		}
		i++;
	}
	return (selected);
}

t_coder *get_minimun(t_manager *manager, int queue_size)
{
	t_coder *selected;

	//pthread_mutex_lock(&manager->mutex);
	selected = get_minimun_locked(manager, queue_size);
	//pthread_mutex_unlock(&manager->mutex);
	return (selected);
}

int     is_on_list(int id, t_coder **list, int n_coders)
{
	int i;

	i = 0;
	while (i < n_coders)
	{
		if (list[i] && id == list[i]->id)
			return (1);
		i++;
	}
	return (0);
}

int     organize(t_manager *manager)
{
	int i;
	t_coder *minimum;

	i = 0;
	while (i < manager->data.number_of_coders)
		manager->priority_queue[i++] = NULL;
	i = 0;
	while (i < manager->data.number_of_coders)
	{
		minimum = get_minimun(manager, i);
		if (minimum == NULL)
			continue;
		printf("%d\n", minimum->id);
		manager->priority_queue[i++] = minimum;
	}
	return (i);
}

void update_queue(t_manager *manager, t_coder *coder)
{
	pthread_mutex_lock(&manager->mutex);
	remove_from_queue(manager, coder);
	organize(manager);
	pthread_mutex_unlock(&manager->mutex);
} */