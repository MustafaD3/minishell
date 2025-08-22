/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haloztur <haloztur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:23:54 by makboga           #+#    #+#             */
/*   Updated: 2025/08/22 22:29:25 by haloztur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	free_builtin_tokens(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->builtin[i])
		free(shell->builtin[i++]);
	i = 0;
	while (shell->tokens[i])
		free(shell->tokens[i++]);
}

static void	free_shell_arrays(t_shell *shell)
{
	int	i;

	free_builtin_tokens(shell);
	if (shell->envp)
	{
		i = 0;
		while (shell->envp[i])
			free(shell->envp[i++]);
		free(shell->envp);
	}
}

void	free_shell(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->prompt)
		free(shell->prompt);
	if (shell->current_dir)
		free(shell->current_dir);
	if (shell->display_info)
		free(shell->display_info);
	if (shell->hostname)
		free(shell->hostname);
	free_shell_arrays(shell);
	free_command(shell);
	clear_history();
}

static void	free_single_command(t_command *command)
{
	t_list	*param;
	t_list	*next_param;

	if (command->contents_p)
	{
		param = command->contents_p;
		while (param)
		{
			next_param = param->next;
			free(param->content);
			free(param);
			param = next_param;
		}
	}
	if (command->redirections)
		free_redirections(command->redirections);
	free(command->command);
	if (command->token)
		free(command->token);
}

void	free_command(t_shell *shell)
{
	t_command	*current;
	t_command	*next;

	if (shell->command_p)
	{
		current = shell->command_p;
		while (current)
		{
			next = current->next;
			free_single_command(current);
			free(current);
			current = next;
		}
	}
}
