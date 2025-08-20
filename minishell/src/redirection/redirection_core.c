/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_core.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 21:00:00 by makboga           #+#    #+#             */
/*   Updated: 2025/08/14 19:20:44 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_redirect	*create_redirect(t_redirect_type type, char *filename)
{
	t_redirect	*redirect;

	redirect = malloc(sizeof(t_redirect));
	if (!redirect)
		return (NULL);
	redirect->type = type;
	redirect->filename = ft_strdup(filename);
	redirect->fd = -1;
	redirect->next = NULL;
	return (redirect);
}

void	add_redirect(t_command *cmd, t_redirect *redirect)
{
	t_redirect	*current;

	if (!cmd || !redirect)
		return ;
	if (!cmd->redirections)
	{
		cmd->redirections = redirect;
		return ;
	}
	current = cmd->redirections;
	while (current->next)
		current = current->next;
	current->next = redirect;
}

void	free_redirections(t_redirect *redirections)
{
	t_redirect	*current;
	t_redirect	*next;

	current = redirections;
	while (current)
	{
		next = current->next;
		if (current->filename)
			free(current->filename);
		if (current->fd > 2)
			close(current->fd);
		free(current);
		current = next;
	}
}

int	is_redirect_token(char *token)
{
	if (!token)
		return (0);
	if (ft_strcmp(token, "<") == 0 || ft_strcmp(token, ">") == 0
		|| ft_strcmp(token, ">>") == 0 || ft_strcmp(token, "<<") == 0)
		return (1);
	return (0);
}

t_redirect_type	get_redirect_type(char *token)
{
	if (!token)
		return (REDIRECT_NONE);
	if (ft_strcmp(token, "<") == 0)
		return (REDIRECT_IN);
	if (ft_strcmp(token, ">") == 0)
		return (REDIRECT_OUT);
	if (ft_strcmp(token, ">>") == 0)
		return (REDIRECT_APPEND);
	if (ft_strcmp(token, "<<") == 0)
		return (REDIRECT_HEREDOC);
	return (REDIRECT_NONE);
}
