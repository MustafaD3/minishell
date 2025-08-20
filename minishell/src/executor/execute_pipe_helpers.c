/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:00:00 by makboga           #+#    #+#             */
/*   Updated: 2025/08/14 16:35:30 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*strip_path(char *cmd)
{
	char	*slash;

	slash = ft_strrchr(cmd, '/');
	if (slash)
		return (slash + 1);
	return (cmd);
}

int	create_pipes(int **pipefds, int n)
{
	int	*pipes;
	int	i;

	pipes = malloc(sizeof(int) * 2 * (n - 1));
	if (!pipes)
		return (-1);
	i = 0;
	while (i < n - 1 && pipe(pipes + i * 2) != -1)
		i++;
	if (i < n - 1)
	{
		free(pipes);
		return (-1);
	}
	*pipefds = pipes;
	return (0);
}

void	setup_child_pipes(int *pipefds, int n, int i)
{
	int	j;

	if (i != 0)
		dup2(pipefds[(i - 1) * 2], STDIN_FILENO);
	if (i != n - 1)
		dup2(pipefds[i * 2 + 1], STDOUT_FILENO);
	j = 0;
	while (j < 2 * (n - 1))
	{
		close(pipefds[j]);
		j++;
	}
}

int	check_pipe_syntax(char *str)
{
	int	i;
	int	len;

	if (!str)
		return (-1);
	len = ft_strlen(str);
	if (len == 0)
		return (-1);
	i = 0;
	while (str[i] && ft_isspace(str[i]))
		i++;
	if (str[i] == '|')
		return (-1);
	i = len - 1;
	while (i >= 0 && ft_isspace(str[i]))
		i--;
	if (i >= 0 && str[i] == '|')
		return (-1);
	return (0);
}

void	close_and_free_pipes(int *pipefds, int n)
{
	int	i;

	if (pipefds)
	{
		i = 0;
		while (i < 2 * (n - 1))
		{
			close(pipefds[i]);
			i++;
		}
		free(pipefds);
	}
}
