/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_helpers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 20:20:00 by makboga           #+#    #+#             */
/*   Updated: 2025/08/14 19:20:44 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	setup_input_redirect(t_redirect *current, int *input_fd)
{
	int	fd;

	if (current->type == REDIRECT_IN)
	{
		fd = open(current->filename, O_RDONLY);
		if (fd == -1)
		{
			perror(current->filename);
			return (-1);
		}
		if (*input_fd != -1)
			close(*input_fd);
		*input_fd = fd;
	}
	else if (current->type == REDIRECT_HEREDOC)
	{
		fd = handle_heredoc(current->filename);
		if (fd == -1)
			return (-1);
		if (*input_fd != -1)
			close(*input_fd);
		*input_fd = fd;
	}
	return (0);
}

int	handle_out_redirect(t_redirect *current, int *output_fd)
{
	int	fd;

	fd = open(current->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror(current->filename);
		return (-1);
	}
	if (*output_fd != -1)
		close(*output_fd);
	*output_fd = fd;
	return (0);
}

int	handle_append_redirect(t_redirect *current, int *output_fd)
{
	int	fd;

	fd = open(current->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror(current->filename);
		return (-1);
	}
	if (*output_fd != -1)
		close(*output_fd);
	*output_fd = fd;
	return (0);
}

int	setup_output_redirect(t_redirect *current, int *output_fd)
{
	if (current->type == REDIRECT_OUT)
		return (handle_out_redirect(current, output_fd));
	else if (current->type == REDIRECT_APPEND)
		return (handle_append_redirect(current, output_fd));
	return (0);
}
