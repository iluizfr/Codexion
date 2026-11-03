/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-frei <lde-frei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/02 17:19:38 by lde-frei          #+#    #+#             */
/*   Updated: 2026/11/03 15:44:07 by lde-frei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Time management functions.

/*give the actual time*/
long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

/*give the begging time - actual time*/
long	elapsed_time(t_manager *manager)
{
	return (get_time() - manager->start_time);
}

/*checks if a coder burned out*/
int	has_burned(t_coder *coder, t_manager *manager)
{
	long	current_time;

	current_time = get_time();
	if (current_time - coder->last_compile >= manager->time_to_burnout)
		return (1);
	return (0);
}
