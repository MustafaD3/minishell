/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_parser.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:45:00 by makboga           #+#    #+#             */
/*   Updated: 2025/08/16 15:47:46 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	cleanup_params(t_parameters *param, t_parameters *to_remove)
{
	free(param->parameter);
	free(param);
	free(to_remove->parameter);
	free(to_remove);
}

static void	update_param_links(t_command *cmd, t_parameters *prev_param,
		t_parameters *next_param)
{
	if (prev_param)
		prev_param->next = next_param;
	else
		cmd->parameters_p = next_param;
}

int	handle_param_redirect(t_command *cmd, t_parameters *param,
		t_parameters *prev_param)
{
	t_redirect		*redirect;
	t_parameters	*next_param;
	t_parameters	*to_remove;

	if (!param->parameter || !is_redirect_token(param->parameter)
		|| !param->next || !param->next->parameter)
		return (0);
	redirect = create_redirect(get_redirect_type(param->parameter),
			param->next->parameter);
	if (!redirect)
		return (0);
	add_redirect(cmd, redirect);
	to_remove = param->next;
	next_param = to_remove->next;
	update_param_links(cmd, prev_param, next_param);
	cleanup_params(param, to_remove);
	return (1);
}

void	handle_token_redirect(t_command *cmd, t_command *next_cmd)
{
	if (next_cmd->token_flag && next_cmd->token
		&& is_redirect_token(next_cmd->token))
	{
		cmd->token = ft_strdup(next_cmd->token);
		cmd->token_flag = 1;
		if (next_cmd->token)
			free(next_cmd->token);
	}
	cmd->next = next_cmd->next;
	if (next_cmd->command)
		free(next_cmd->command);
	free(next_cmd);
}

void	process_redirections(t_shell *shell)
{
	t_command	*cmd;

	cmd = shell->command_p;
	while (cmd)
	{
		if (cmd->parameters_p)
			process_param_redirections(cmd);
		process_token_redirections(cmd);
		cmd = cmd->next;
	}
}
