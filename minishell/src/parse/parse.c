/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkilic344@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 07:09:35 by mdalkili          #+#    #+#             */
/*   Updated: 2025/08/21 00:37:01 by mdalkili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	append(t_shell *shell, char *str, int *command, t_command **temp)
{
	char			*temp_str;
	int				result;
	int				token_result;
	t_parameters	*new_param;

	if (!str || !shell)
		return ;
	result = prompt_type_control_loop(shell->builtin, 1, str);
	token_result = prompt_type_control_loop(shell->tokens, 0, str);
	if (ft_strlen(str) == 0)
		return ;
	else if (!*command && (result == 1 || result == 2 || result == 3))
	{
		temp_str = ft_strdup(str);
		append_command(shell, temp_str,
			prompt_type_control_loop(shell->builtin, 1, str), temp);
		free(temp_str);
		*command = 1;
	}
	else if (token_result == 4)
	{
		if (*temp)
		{
			new_param = malloc(sizeof(t_parameters));
			new_param->parameter = ft_strdup(str);
			new_param->next = NULL;
			append_parameter(new_param, temp);
		}
	}
	else
	{
		if (*temp)
		{
			new_param = malloc(sizeof(t_parameters));
			new_param->parameter = ft_strdup(str);
			new_param->next = NULL;
			append_parameter(new_param, temp);
		}
	}
}

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

void	parse_prompt(t_shell *shell)
{
	char		*temp_prompt;
	char		*current_option;
	char		*start;
	char		*(*parse_func)(char **, t_shell *);
	int			command;
	t_command	*command_temp_p;

	command = 0;
	free_command(shell);
	command_temp_p = NULL;
	shell->command_p = command_temp_p;
	temp_prompt = ft_strdup(shell->prompt);
	start = temp_prompt;
	while (temp_prompt && *temp_prompt)
	{
		parse_func = NULL;
		if (*temp_prompt == '\'')
			parse_func = single_quote_control;
		else if (*temp_prompt == '"')
			parse_func = double_quote_control;
		else if (*temp_prompt == '>' || *temp_prompt == '<')
			parse_func = get_redirect_operator;
		else if (*temp_prompt == '|')
		{
			temp_prompt++;
			command = 0;
			command_temp_p = NULL;
			continue ;
		}
		else if (!is_whitespace(*temp_prompt))
			parse_func = get_characters;
		if (parse_func)
		{
			current_option = parse_func(&temp_prompt, shell);
			if (current_option != NULL)
			{
				if (!command_temp_p)
				{
					append(shell, current_option, &command, &command_temp_p);
					shell->command_p = command_temp_p;
				}
				else
					append(shell, current_option, &command, &shell->command_p);
			}
			if (current_option)
				free(current_option);
			continue ;
		}
		temp_prompt++;
	}
	shell->command_p = command_temp_p;
	process_redirections(shell);
	free(start);
}

int	get_prompt(t_shell *shell)
{
	shell->prompt = set_and_free(shell->prompt, readline(shell->display_info));
	if (!shell->prompt)
	{
		printf("exit\n");
		return (0);
	}
	if (shell->prompt && *shell->prompt)
	{
		parse_prompt(shell);
		add_history(shell->prompt);
	}
	else
		shell->command_p = NULL;
	return (1);
}
