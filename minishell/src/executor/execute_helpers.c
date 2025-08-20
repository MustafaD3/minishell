/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 16:01:50 by makboga           #+#    #+#             */
/*   Updated: 2025/08/17 20:29:25 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	has_pipe_outside_quotes(char *str)
{
	int	i;
	int	in_single_quote;
	int	in_double_quote;

	i = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (str[i] == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else if (str[i] == '|' && !in_single_quote && !in_double_quote)
			return (1);
		i++;
	}
	return (0);
}

void	handle_builtin_command(t_shell *shell, char **params, char *cmd_name)
{
	if (ft_strcmp(cmd_name, "echo") == 0)
		shell->last_exit_code = builtin_echo(params);
	else if (ft_strcmp(cmd_name, "pwd") == 0)
		shell->last_exit_code = builtin_pwd();
	else if (ft_strcmp(cmd_name, "exit") == 0)
		shell->last_exit_code = builtin_exit(shell, params,
				shell->last_exit_code);
	else if (ft_strcmp(cmd_name, "env") == 0)
		shell->last_exit_code = builtin_env(shell->envp);
	else if (ft_strcmp(cmd_name, "cd") == 0)
		shell->last_exit_code = builtin_cd(shell, params);
	else if (ft_strcmp(cmd_name, "export") == 0)
		shell->last_exit_code = builtin_export(&shell->envp, params);
	else if (ft_strcmp(cmd_name, "unset") == 0)
		shell->last_exit_code = builtin_unset(shell, params[1]);
	else
		printf("command not found: %s\n", cmd_name);
}

char	**get_params(t_command *command)
{
	char	**params;
	int		count;

	count = count_parameters(command);
	params = malloc(sizeof(char *) * (count + 2));
	if (!params)
		return (NULL);
	fill_params_array(params, command, count);
	return (params);
}

int	count_parameters(t_command *command)
{
	t_parameters	*tmp;
	int				count;

	count = 0;
	tmp = command->parameters_p;
	while (tmp && tmp->parameter)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

void	fill_params_array(char **params, t_command *command, int count)
{
	t_parameters	*tmp;
	int				i;

	params[0] = command->command;
	i = 1;
	tmp = command->parameters_p;
	while (tmp && tmp->parameter && i <= count)
	{
		params[i] = tmp->parameter;
		i++;
		tmp = tmp->next;
	}
	params[i] = NULL;
}
