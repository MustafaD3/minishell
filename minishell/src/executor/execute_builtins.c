/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkilic344@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 22:10:03 by makboga           #+#    #+#             */
/*   Updated: 2025/08/21 00:29:44 by mdalkili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	handle_command_execution(t_shell *shell, char **params)
{
	char	*cmd_name;

	if (shell->command_p->builtin == 2 || shell->command_p->builtin == 1)
		shell->last_exit_code = run(shell->command_p, params, shell);
	else if (shell->command_p->builtin == 3)
	{
		cmd_name = ft_strrchr(shell->command_p->command, '/');
		if (cmd_name)
			cmd_name++;
		else
			cmd_name = shell->command_p->command;
		handle_builtin_command(shell, params, cmd_name);
	}
}

static void	restore_std_fds(int original_stdin, int original_stdout)
{
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdin);
	close(original_stdout);
}

static void	execute_with_redirections(t_shell *shell)
{
	char	**params;
	int		original_stdin;
	int		original_stdout;

	original_stdin = dup(STDIN_FILENO);
	original_stdout = dup(STDOUT_FILENO);
	if (setup_redirections(shell->command_p) == -1)
	{
		shell->last_exit_code = 1;
		restore_std_fds(original_stdin, original_stdout);
		return ;
	}
	params = get_params(shell->command_p);
	handle_command_execution(shell, params);
	free(params);
	restore_std_fds(original_stdin, original_stdout);
}

void	handle_single_command_exec(t_shell *shell)
{
	char	**params;

	if (!shell->command_p || !shell->command_p->redirections)
	{
		params = get_params(shell->command_p);
		handle_command_execution(shell, params);
		free(params);
		return ;
	}
	execute_with_redirections(shell);
}

void	execute_single_command(t_shell *shell)
{
	char	**params;

	if (!shell || !shell->command_p)
		return ;
	params = get_params(shell->command_p);
	handle_command_execution(shell, params);
	free(params);
}
