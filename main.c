/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-frei <lde-frei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/02 17:19:47 by lde-frei          #+#    #+#             */
/*   Updated: 2026/11/02 17:19:48 by lde-frei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	t_data	data;

	printf("========== CODEXION ==========\n\n");
	if (!parser(argc, argv))
	{
		printf("Invalid input.\n");
		return (1);
	}
	data = init_data(argv);
	run(data);
	printf("\n========== CODEXION ==========\n");
	return (0);
}
