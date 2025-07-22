/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 19:06:02 by mdalkili          #+#    #+#             */
/*   Updated: 2025/07/22 20:24:23 by mdalkili         ###   ########.fr       */
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
	else if (*temp)  // *temp'in NULL olmadığından emin ol
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
int prompt_type_control_loop(char **control_list,int type,char *str)
{
	int i;
	
	i = 0;
	if(type)
	{
		while(control_list && control_list[i])
		{
			if (ft_strcmp(control_list[i], str) == 0)
				return 3;
			i++;
		}
		if(access(str, F_OK) == 0)
			return 2;
		else
			return 1;
	}
	else{
		while (control_list[i])
    	{
			if (ft_strcmp(control_list[i], str) == 0)
				return 4;
			i++;
    	}
	}
	return 0;
}