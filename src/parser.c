/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-frei <lde-frei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/02 17:19:27 by lde-frei          #+#    #+#             */
/*   Updated: 2026/11/03 16:13:54 by lde-frei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	verify_arg(char *arg)
{
	int	i;

	i = 0;
	if (!arg || !arg[0])
		return (0);
	while (arg[i])
	{
		if (!isdigit((unsigned char)arg[i]))
			return (0);
		i++;
	}
	return (1);
}

int	verify_args(char **argv)
{
	int	i;

	i = 1;
	while (i < 8 && argv[i])
	{
		if (!verify_arg(argv[i]))
		{
			printf("'%s' must be a positive number.\n", argv[i]);
			return (-1);
		}
		i++;
	}
	return (0);
}

int	parser(int argc, char **argv)
{
	if (argc != 9)
	{
		printf("The number of arguments given is invalid.\n");
		printf(
			"Usage example: './codexion 5 600 100 100 100 3 100 \"edf\"'\n");
		return (0);
	}
	if (verify_args(argv) < 0)
		return (0);
	if ((strcmp(argv[8], "fifo") != 0) && (strcmp(argv[8], "edf") != 0))
	{
		printf("Invalid argument, unknow schedular '%s'.\n", argv[8]);
		return (0);
	}
	return (1);
}
