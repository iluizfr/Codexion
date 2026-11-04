/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-frei <lde-frei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/02 17:19:03 by lde-frei          #+#    #+#             */
/*   Updated: 2026/11/03 18:40:40 by lde-frei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	especial_case(t_coder *coder)
{
	pthread_mutex_lock(&coder->left_dongle->mutex);
	log_message(coder->manager, coder->id, "has taken a dongle", coder->color);
	while (!is_stopped(coder->manager))
		usleep(1000);
	pthread_mutex_unlock(&coder->left_dongle->mutex);
}

/*coders routine*/
void	*coder_routine(void *arg)
{
	t_coder		*coder;

	coder = (t_coder *)arg;
	if (coder->manager->data.number_of_coders == 1)
		especial_case(coder);
	else
	{
		while (need_to_compile(coder, coder->compiles_required)
			&& !is_stopped(coder->manager))
		{
			compile(coder, coder->manager);
			if (is_stopped(coder->manager))
				break ;
			debug(coder);
			if (is_stopped(coder->manager))
				break ;
			refactor(coder);
		}
	}
	pthread_mutex_lock(&coder->manager->mutex);
	if (!coder->burned && coder->compile_count == coder->compiles_required)
		coder->finished = 1;
	pthread_mutex_unlock(&coder->manager->mutex);
	return (NULL);
}
