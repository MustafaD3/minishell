/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_process.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 20:10:00 by makboga           #+#    #+#             */
/*   Updated: 2025/08/14 19:20:44 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	validate_input_file(t_redirect *current)
{
	if (access(current->filename, F_OK) == -1
		|| access(current->filename, R_OK) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(current->filename, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (-1);
	}
	return (0);
}

static int	validate_output_file(t_redirect *current)
{
	int	fd;
	int	flags;

	if (current->type == REDIRECT_OUT)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else
		flags = O_WRONLY | O_CREAT | O_APPEND;
	fd = open(current->filename, flags, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(current->filename, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		return (-1);
	}
	close(fd);
	return (0);
}

int	validate_redirections(t_command *cmd)
{
	t_redirect	*current;

	current = cmd->redirections;
	while (current)
	{
		if (current->type == REDIRECT_IN)
		{
			if (validate_input_file(current) == -1)
				return (-1);
		}
		else if (current->type == REDIRECT_OUT
			|| current->type == REDIRECT_APPEND)
		{
			if (validate_output_file(current) == -1)
				return (-1);
		}
		current = current->next;
	}
	return (0);
}

static int	handle_redirections(t_command *cmd, int *input_fd, int *output_fd)
{
	t_redirect	*current;

	current = cmd->redirections;
	while (current)
	{
		if (current->type == REDIRECT_IN || current->type == REDIRECT_HEREDOC)
		{
			if (process_input_redirections(current, input_fd, output_fd) == -1)
				return (-1);
		}
		else if (current->type == REDIRECT_OUT
			|| current->type == REDIRECT_APPEND)
		{
			if (process_output_redirections(current, input_fd, output_fd) == -1)
				return (-1);
		}
		current = current->next;
	}
	return (0);
}

int	setup_redirections(t_command *cmd)
{
	int	input_fd;
	int	output_fd;

	if (!cmd || !cmd->redirections)
		return (0);
	if (validate_redirections(cmd) == -1)
		return (-1);
	input_fd = -1;
	output_fd = -1;
	if (handle_redirections(cmd, &input_fd, &output_fd) == -1)
		return (-1);
	return (apply_redirections(input_fd, output_fd));
}
