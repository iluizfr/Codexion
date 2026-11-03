/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-frei <lde-frei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 14:33:49 by lde-frei          #+#    #+#             */
/*   Updated: 2026/11/03 15:16:02 by lde-frei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	wait_dongles(t_coder *coder, t_dongle **first, t_dongle **second)
{
	while (!is_stopped(coder->manager))
	{
		pthread_mutex_lock(&(*first)->mutex);
		if (get_time() < (*first)->avaliable_at)
		{
			pthread_mutex_unlock(&(*first)->mutex);
			usleep(1000);
			continue ;
		}
		pthread_mutex_lock(&(*second)->mutex);
		if (get_time() < (*second)->avaliable_at)
		{
			pthread_mutex_unlock(&(*second)->mutex);
			pthread_mutex_unlock(&(*first)->mutex);
			usleep(1000);
			continue ;
		}
		return (1);
	}
	pthread_mutex_unlock(&(*second)->mutex);
	pthread_mutex_unlock(&(*first)->mutex);
	return (0);
}
