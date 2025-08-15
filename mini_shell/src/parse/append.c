/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkilic344@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 19:06:02 by mdalkili          #+#    #+#             */
/*   Updated: 2025/08/15 04:10:40 by mdalkili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"


void append_command(t_shell *shell, char *str,int builtin, t_command **temp)
{
	if (!shell->command_p)
	{
		shell->command_p = malloc(sizeof(t_command));
		shell->command_p->command = ft_strdup(str);
		shell->command_p->next = NULL;
		shell->command_p->parameters_p = NULL;
		shell->command_p->token = NULL;
		shell->command_p->flag = 1;
		shell->command_p->token_flag = 0;
		shell->command_p->builtin = builtin;
		*temp = shell->command_p;
	}
	else if (*temp)
	{
		(*temp)->next = malloc(sizeof(t_command));
		(*temp)->next->command = ft_strdup(str);
		(*temp)->next->next = NULL;
		(*temp)->next->parameters_p = NULL;
		(*temp)->next->token = NULL;
		(*temp)->next->token_flag = 0;
		(*temp)->next->builtin = builtin;
		(*temp)->next->flag = 1;
		*temp = (*temp)->next;
	}
	else
	{
		t_command *last = shell->command_p;
		while (last && last->next)
			last = last->next;
		if (last)
		{
			last->next = malloc(sizeof(t_command));
			last->next->command = ft_strdup(str);
			last->next->next = NULL;
			last->next->parameters_p = NULL;
			last->next->token = NULL;
			last->next->token_flag = 0;
			last->next->builtin = builtin;
			last->next->flag = 1;
			*temp = last->next;
		}
	}
}
void append_parameter(t_parameters *new_param, t_command **temp)
{
	if (!(*temp)->parameters_p)
		(*temp)->parameters_p = new_param;
	else
	{
		t_parameters *p = (*temp)->parameters_p;
		while (p->next)
			p = p->next;
		p->next = new_param;
	}

}

void append_token(char *token, t_command **temp)
{
	(*temp)->token = ft_strdup(token);
	(*temp)->token_flag = 1;
}
