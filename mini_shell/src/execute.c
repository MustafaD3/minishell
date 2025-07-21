/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkilic344@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 18:41:36 by makboga           #+#    #+#             */
/*   Updated: 2025/07/21 20:57:01 by mdalkili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_path(char *cmd, char **envp)
{
	char	**paths;
	char	*full_path;
	int		i;

	if (!cmd || !envp)
		return (NULL);
	while (*envp && ft_strncmp(*envp, "PATH=", 5) != 0)
		envp++;
	if (!*envp)
		return (NULL);
	paths = ft_split(*envp + 5, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin_free(full_path, cmd);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	ft_free_split(paths);
	return (NULL);
}

void close_pipes(int *pipefds, int count)
{
    int i = 0;
    while (i < count)
    {
        close(pipefds[i]);
        i++;
    }
}

void setup_child_fds(int *pipefds, int n, int i)
{
    if (i != 0)
        dup2(pipefds[(i - 1) * 2], STDIN_FILENO);
    if (i != n - 1)
        dup2(pipefds[i * 2 + 1], STDOUT_FILENO);

    close_pipes(pipefds, 2 * (n - 1));
}

void execute_commands(t_shell *shell, char **commands, int n)
{
	int		*pipefds;
	pid_t	pid;
	int		i;
	char	**argv;
	char	*path;

	if (!commands || n < 1)
		return;

	pipefds = NULL;
	if (n > 1)
	{
		pipefds = malloc(sizeof(int) * 2 * (n - 1));
		if (!pipefds)
			return;
		i = 0;
		while (i < n - 1)
		{
			if (pipe(pipefds + i * 2) == -1)
			{
				perror("pipe");
				close_pipes(pipefds, i * 2);
				free(pipefds);
				return;
			}
			i++;
		}
	}

	i = 0;
	while (i < n)
	{
		pid = fork();
		if (pid == 0)
		{
			if (pipefds)
				setup_child_fds(pipefds, n, i);

			argv = ft_split(commands[i], ' ');
			if (!argv || !argv[0])
				exit(1);

			path = get_path(argv[0], shell->envp);
			if (!path)
			{
				ft_putstr_fd(argv[0], 2);
				ft_putstr_fd(": command not found\n", 2);
				free_argv(argv);
				exit(127);
			}
			execve(path, argv, shell->envp);
			perror("execve failed");
			free(path);
			free_argv(argv);
			exit(1);
		}
		else if (pid < 0)
		{
			perror("fork");
			break;
		}
		i++;
	}

	if (pipefds)
	{
		close_pipes(pipefds, 2 * (n - 1));
		free(pipefds);
	}

	i = 0;
	while (i < n)
	{
		wait(NULL);
		i++;
	}
}

void execute_parsed_commands(t_shell *shell)
{
	t_command *current_cmd;
	char **argv;
	int argc;
	t_parameters *param;

	if (!shell || !shell->command_p)
		return;

	current_cmd = shell->command_p;
	while (current_cmd)
	{
		if (current_cmd->command)
		{
			// Count arguments
			argc = 1; // Command itself
			param = current_cmd->parameters_p;
			while (param)
			{
				argc++;
				param = param->next;
			}

			// Build argv array
			argv = malloc(sizeof(char *) * (argc + 1));
			if (!argv)
				return;

			argv[0] = current_cmd->command;
			argc = 1;
			param = current_cmd->parameters_p;
			while (param)
			{
				argv[argc] = param->parameter;
				argc++;
				param = param->next;
			}
			argv[argc] = NULL;

			// Execute command
			if (!builtin_exe(shell, argv))
			{
				// Not a builtin, try external command
				char *path = get_path(argv[0], shell->envp);
				if (path)
				{
					pid_t pid = fork();
					if (pid == 0)
					{
						execve(path, argv, shell->envp);
						exit(1);
					}
					else if (pid > 0)
					{
						waitpid(pid, NULL, 0);
					}
					free(path);
				}
				else
				{
					printf("minishell: %s: command not found\n", argv[0]);
				}
			}

			free(argv);
		}
		current_cmd = current_cmd->next;
	}
}
