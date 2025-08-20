/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:47:57 by makboga           #+#    #+#             */
/*   Updated: 2025/08/17 20:28:23 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	is_numeric(char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	i = 0;
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	builtin_exit(t_shell *shell, char **argv, int last_exit_code)
{
	int	status;

	ft_putstr_fd("exit\n", 1);
	if (!argv[1])
		status = last_exit_code;
	else if (argv[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		status = 1;
	}
	else if (!is_numeric(argv[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		status = 2;
	}
	else
	{
		status = ft_atoi(argv[1]);
	}
	shell->last_exit_code = status;
	free_shell(shell);
	exit(status);
}
