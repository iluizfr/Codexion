/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-frei <lde-frei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/02 17:18:56 by lde-frei          #+#    #+#             */
/*   Updated: 2026/11/05 02:12:13 by lde-frei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*The refact action perfomed by the coder*/
void	refactor(t_coder *coder)
{
	if (is_stopped(coder->manager))
		return ;
	log_message(coder->manager, coder->id, "is refactoring", coder->color);
	usleep(coder->manager->data.time_to_refactor * 1000);
}

/*The debug action performed by the coder*/
void	debug(t_coder *coder)
{
	if (is_stopped(coder->manager))
		return ;
	log_message(coder->manager, coder->id, "is debugging", coder->color);
	usleep(coder->manager->data.time_to_debug * 1000);
}

/* Unlocks the dongle and updates the time of avaliability
   of the dongle.
*/
void	release_dongle(t_dongle *dongle, t_manager *manager)
{
	t_priority_queue	*queue;

	dongle->avaliable_at = get_time() + dongle->dongle_cooldown;
	pthread_mutex_unlock(&dongle->mutex);
	queue = &manager->priority_queue;
	pthread_mutex_lock(&queue->mutex);
	pthread_cond_broadcast(&queue->cond);
	pthread_mutex_unlock(&queue->mutex);
}

/* checks the order to acquire the dongle and waits the dongle
   cooldown.
*/
int	acquire_dongles(t_coder *coder, t_dongle **first, t_dongle **second)
{
	if (coder->left_dongle->dongle_id < coder->right_dongle->dongle_id)
	{
		*first = coder->left_dongle;
		*second = coder->right_dongle;
	}
	else
	{
		*first = coder->right_dongle;
		*second = coder->left_dongle;
	}
	if (wait_dongles(coder, first, second))
		return (1);
	return (0);
}

/*The compile action performed by the coder*/
int	compile(t_coder *coder, t_manager *manager)
{
	t_dongle	*first;
	t_dongle	*second;

	if (!acquire_dongles(coder, &first, &second))
		return (0);
	log_message(manager, coder->id, "has taken a dongle", coder->color);
	log_message(manager, coder->id, "has taken a dongle", coder->color);
	pthread_mutex_lock(&manager->mutex);
	if (manager->stop)
	{
		pthread_mutex_unlock(&manager->mutex);
		pthread_mutex_unlock(&second->mutex);
		pthread_mutex_unlock(&first->mutex);
		return (0);
	}
	coder->last_compile = get_time();
	coder->compile_count++;
	pthread_mutex_unlock(&manager->mutex);
	log_message(manager, coder->id, "is compiling", coder->color);
	usleep(manager->data.time_to_compile * 1000);
	release_dongle(second, manager);
	release_dongle(first, manager);
	return (1);
}
