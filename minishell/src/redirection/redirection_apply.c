/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_apply.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 20:35:00 by makboga           #+#    #+#             */
/*   Updated: 2025/08/14 19:20:44 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	apply_input_redirect(int input_fd, int output_fd)
{
	if (dup2(input_fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(input_fd);
		if (output_fd != -1)
			close(output_fd);
		return (-1);
	}
	close(input_fd);
	return (0);
}

static int	apply_output_redirect(int output_fd)
{
	if (dup2(output_fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(output_fd);
		return (-1);
	}
	close(output_fd);
	return (0);
}

int	apply_redirections(int input_fd, int output_fd)
{
	if (input_fd != -1)
	{
		if (apply_input_redirect(input_fd, output_fd) == -1)
			return (-1);
	}
	if (output_fd != -1)
	{
		if (apply_output_redirect(output_fd) == -1)
			return (-1);
	}
	return (0);
}

void	process_param_redirections(t_command *cmd)
{
	t_parameters	*param;
	t_parameters	*prev_param;

	param = cmd->parameters_p;
	prev_param = NULL;
	while (param)
	{
		if (handle_param_redirect(cmd, param, prev_param))
		{
			if (prev_param)
				param = prev_param->next;
			else
				param = cmd->parameters_p;
			continue ;
		}
		prev_param = param;
		param = param->next;
	}
}

void	process_token_redirections(t_command *cmd)
{
	t_command	*next_cmd;
	t_redirect	*redirect;

	if (!cmd->token_flag || !cmd->token || !is_redirect_token(cmd->token))
		return ;
	next_cmd = cmd->next;
	if (!next_cmd || !next_cmd->command)
		return ;
	redirect = create_redirect(get_redirect_type(cmd->token),
			next_cmd->command);
	if (!redirect)
		return ;
	add_redirect(cmd, redirect);
	free(cmd->token);
	cmd->token = NULL;
	cmd->token_flag = 0;
	handle_token_redirect(cmd, next_cmd);
}
