/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-frei <lde-frei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 14:33:49 by lde-frei          #+#    #+#             */
/*   Updated: 2026/11/05 02:32:46 by lde-frei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	lock_dongles(t_dongle *first, t_dongle *second)
{
	pthread_mutex_lock(&first->mutex);
	pthread_mutex_lock(&second->mutex);
	if (get_time() < first->avaliable_at
		|| get_time() < second->avaliable_at)
	{
		pthread_mutex_unlock(&second->mutex);
		pthread_mutex_unlock(&first->mutex);
		return (0);
	}
	return (1);
}

static int	is_first_in_queue(t_priority_queue *queue, t_coder *coder)
{
	if (queue->size == 0)
		return (0);
	return (queue->heap[0].coder == coder);
}

static int	queue_coder(t_coder *coder, t_priority_queue *queue)
{
	long	deadline;

	pthread_mutex_lock(&coder->manager->mutex);
	deadline = coder->last_compile + coder->manager->time_to_burnout;
	pthread_mutex_unlock(&coder->manager->mutex);
	pthread_mutex_lock(&queue->mutex);
	if (!enqueue(queue, coder, deadline))
	{
		pthread_mutex_unlock(&queue->mutex);
		return (0);
	}
	pthread_mutex_unlock(&queue->mutex);
	return (1);
}

static void	remove_coder(t_priority_queue *queue, t_coder *coder)
{
	pthread_mutex_lock(&queue->mutex);
	remove_from_queue(queue, coder);
	if (queue->acquiring == coder)
		queue->acquiring = NULL;
	pthread_cond_broadcast(&queue->cond);
	pthread_mutex_unlock(&queue->mutex);
}

static void	release_reservation(t_priority_queue *queue, t_coder *coder)
{
	pthread_mutex_lock(&queue->mutex);
	if (queue->acquiring == coder)
		queue->acquiring = NULL;
	pthread_cond_broadcast(&queue->cond);
	pthread_mutex_unlock(&queue->mutex);
}

int	wait_dongles(t_coder *coder, t_dongle **first, t_dongle **second)
{
	t_priority_queue	*queue;

	queue = &coder->manager->priority_queue;
	if (!queue_coder(coder, queue))
		return (0);
	while (1)
	{
		pthread_mutex_lock(&queue->mutex);
		if (is_stopped(coder->manager))
		{
			pthread_mutex_unlock(&queue->mutex);
			break ;
		}
		if (queue->acquiring || !is_first_in_queue(queue, coder))
		{
			pthread_cond_wait(&queue->cond, &queue->mutex);
			pthread_mutex_unlock(&queue->mutex);
			continue ;
		}
		queue->acquiring = coder;
		pthread_mutex_unlock(&queue->mutex);
		if (!lock_dongles(*first, *second))
		{
			release_reservation(queue, coder);
			usleep(1000);
			continue ;
		}
		if (is_stopped(coder->manager))
		{
			pthread_mutex_unlock(&(*second)->mutex);
			pthread_mutex_unlock(&(*first)->mutex);
			remove_coder(queue, coder);
			return (0);
		}
		remove_coder(queue, coder);
		return (1);
	}
	remove_coder(queue, coder);
	return (0);
}
