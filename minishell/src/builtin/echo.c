/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:20:56 by makboga           #+#    #+#             */
/*   Updated: 2025/08/16 15:42:37 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	is_n_option(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-')
		return (0);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	builtin_echo(char **argv)
{
	int	i;
	int	n_flag;
	int	first_arg;

	i = 1;
	n_flag = 0;
	first_arg = 1;
	while (argv[i] && is_n_option(argv[i]))
	{
		n_flag = 1;
		i++;
	}
	while (argv[i])
	{
		if (!first_arg)
			write(1, " ", 1);
		write(1, argv[i], ft_strlen(argv[i]));
		first_arg = 0;
		i++;
	}
	if (!n_flag)
		write(1, "\n", 1);
	return (0);
}
