/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkilic344@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:20:56 by makboga           #+#    #+#             */
/*   Updated: 2025/08/04 19:08:50 by mdalkili         ###   ########.fr       */
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

	i = 1;
	n_flag = 0;
	while (argv[i] && is_n_option(argv[i]))
	{
		n_flag = 1;
		i++;
	}
	while (argv[i])
	{
		if (write(STDOUT_FILENO, argv[i], strlen(argv[i])) < 0)
            return (1);
		if (argv[i + 1])
		{
            if (write(STDOUT_FILENO, " ", 1) < 0)
                return (1);
        }
		i++;
	}
	if (!n_flag)
		if (write(STDOUT_FILENO, "\n", 1) < 0)
            return (1);
	return (0);
}
