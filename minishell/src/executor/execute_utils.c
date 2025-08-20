/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkilic344@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:59:51 by makboga           #+#    #+#             */
/*   Updated: 2025/08/19 00:37:08 by mdalkili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*check_path_in_dirs(char **paths, char *cmd)
{
	char	*temp;
	char	*full_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*get_path(char *cmd)
{
	char	*path_env;
	char	**paths;
	char	*result;

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	result = check_path_in_dirs(paths, cmd);
	free_array(paths);
	return (result);
}

void	execute_commands(t_shell *shell, char **commands, int n)
{
	int	*pipefds;

	if (n < 2)
		return ;
	if (create_pipes(&pipefds, n) == -1)
		return ;
	fork_and_execute(shell, commands, n, pipefds);
}

void	cleanup_and_wait(int *pipefds, int n, pid_t last_pid, t_shell *shell)
{
	int	status;
	int	i;

	close_and_free_pipes(pipefds, n);
	if (last_pid != -1)
	{
		waitpid(last_pid, &status, 0);
		if (WIFEXITED(status))
			shell->last_exit_code = WEXITSTATUS(status);
	}
	i = 0;
	while (i < n - 1)
	{
		wait(NULL);
		i++;
	}
}

void	execute_child_process(t_shell *shell, char *command,
		t_pipe_info pipe_info)
{
	t_shell	tmp_shell;
	char	*trimmed_command;

	ft_memset(&tmp_shell, 0, sizeof(t_shell));
	tmp_shell.envp = shell->envp;
	trimmed_command = ft_strtrim(command, " \t\n\r");
	tmp_shell.prompt = trimmed_command;
	parse_prompt(&tmp_shell);
	setup_child_pipes(pipe_info.pipefds, pipe_info.n, pipe_info.i);
	if (tmp_shell.command_p && tmp_shell.command_p->redirections)
	{
		if (setup_redirections(tmp_shell.command_p) != 0)
		{
			free_shell(&tmp_shell);
			exit(1);
		}
	}
	execute_single_command(&tmp_shell);
	free_shell(&tmp_shell);
	exit(tmp_shell.last_exit_code);
}
