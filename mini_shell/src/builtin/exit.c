/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkilic344@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:47:57 by makboga           #+#    #+#             */
/*   Updated: 2025/08/06 04:18:18 by mdalkili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	builtin_exit(t_shell *shell, char **params)
{
	int exit_code = 0;
	
	write(STDOUT_FILENO, "exit\n", 5);
	
	if (params && params[1])
	{
		// Check if argument is numeric
		char *endptr;
		long long num = strtoll(params[1], &endptr, 10);
		
		if (*endptr != '\0')
		{
			// Non-numeric argument
			printf("minishell: exit: %s: numeric argument required\n", params[1]);
			exit_code = 255;
		}
		else
		{
			// Handle overflow/underflow for exit codes
			exit_code = (int)(num % 256);
			if (exit_code < 0)
				exit_code += 256;
		}
		
		if (params[2])
		{
			// Too many arguments
			printf("minishell: exit: too many arguments\n");
			return 1; // Don't exit, just return error
		}
	}
	
	free_shell(shell);
	exit(exit_code);
}

