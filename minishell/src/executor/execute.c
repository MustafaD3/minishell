/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkilic344@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:59:51 by makboga           #+#    #+#             */
/*   Updated: 2025/08/22 23:59:19 by mdalkili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	execute(t_shell *shell)
{
	execute_main(shell);
}

void	execute_main(t_shell *shell)
{
	if (has_pipe_outside_quotes(shell->prompt))
		handle_pipe_commands(shell);
	else
		handle_single_command_exec(shell);
}

void	handle_pipe_commands(t_shell *shell)
{
	char	**commands_pipes;
	int		i_pipes;

	if (check_pipe_syntax(shell->prompt) != 0)
		return ;
	commands_pipes = ft_split(shell->prompt, '|');
	i_pipes = 0;
	while (commands_pipes[i_pipes])
		i_pipes++;
	if (i_pipes > 1)
		execute_commands(shell, commands_pipes, i_pipes);
	free_array(commands_pipes);
}

void	fork_and_execute(t_shell *shell, char **commands, int n, int *pipefds)
{
	pid_t		pids[1024];
	int			i;
	t_pipe_info	pipe_info;

	i = 0;
	while (i < n)
	{
		pids[i] = fork();
		if (pids[i] == 0)
		{
			pipe_info.pipefds = pipefds;
			pipe_info.n = n;
			pipe_info.i = i;
			execute_child_process(shell, commands[i], pipe_info);
		}
		else if (pids[i] < 0)
			break ;
		i++;
	}
	cleanup_and_wait(pipefds, n, pids[n - 1], shell);
}
