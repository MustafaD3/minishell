/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkilic344@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:06:55 by makboga           #+#    #+#             */
/*   Updated: 2025/08/29 18:46:48 by mdalkili         ###   ########.fr       */
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

static char	*get_env_var(char **envp, char *var_name)
{
	int	i;
	int	len;

	if (!envp || !var_name)
		return (NULL);
	len = ft_strlen(var_name);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var_name, len) == 0 && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

char	*get_path(char *cmd, char **envp)
{
	char	*path_env;
	char	**paths;
	char	*result;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_env = get_env_var(envp, "PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	result = check_path_in_dirs(paths, cmd);
	ft_free_split(paths);
	return (result);
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
		else if (WIFSIGNALED(status))
			shell->last_exit_code = 128 + WTERMSIG(status);
	}
	i = 0;
	while (i < n - 1)
	{
		wait(NULL);
		i++;
	}
}
