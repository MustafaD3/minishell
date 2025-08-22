/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkilic344@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:03:40 by mdalkili          #+#    #+#             */
/*   Updated: 2025/08/23 01:04:37 by mdalkili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	prompt_type_control_loop(char **control_list, int type, char *str)
{
	int	i;

	i = 0;
	if (type)
	{
		while (control_list && control_list[i])
		{
			if (ft_strcmp(control_list[i++], str) == 0)
				return (3);
		}
		if (access(str, F_OK) == 0)
			return (2);
		else
			return (1);
	}
	else
	{
		while (control_list[i])
		{
			if (ft_strcmp(control_list[i], str) == 0)
				return (4);
			i++;
		}
	}
	return (0);
}

char	*re_control(char *tmp, char *result, char **prompt, t_shell *shell)
{
	if (**prompt == '\'')
	{
		tmp = result;
		result = ft_strjoin(tmp, single_quote_control(prompt, shell));
		if (tmp)
			free(tmp);
	}
	else if (**prompt == '"')
	{
		tmp = result;
		result = ft_strjoin(tmp, double_quote_control(prompt, shell));
		if (tmp)
			free(tmp);
	}
	else if (**prompt && !ft_isspace(**prompt))
	{
		tmp = result;
		result = ft_strjoin(tmp, get_characters(prompt, shell));
		if (tmp)
			free(tmp);
	}
	if (result == NULL)
		return (ft_strdup(""));
	return (result);
}

void	reset_parse(t_parse *parse, t_shell *shell)
{
	if (!parse)
		return ;
	parse->temp_prompt = NULL;
	parse->current_option = NULL;
	parse->start = NULL;
	parse->parse_func = NULL;
	parse->command = 0;
	parse->had_pipe = 0;
	parse->command_temp_p = NULL;
	shell->parse_error = 0;
	free_command(shell);
	shell->command_p = parse->command_temp_p;
	parse->temp_prompt = ft_strdup(shell->prompt);
	parse->start = parse->temp_prompt;
}

int	run_parse(t_parse *parse, t_shell *shell)
{
	parse->current_option = parse->parse_func(&parse->temp_prompt, shell);
	if (parse->current_option != NULL)
	{
		if (!parse->command_temp_p)
		{
			append(shell, parse->current_option, &parse->command,
				&parse->command_temp_p);
			shell->command_p = parse->command_temp_p;
		}
		else
			append(shell, parse->current_option, &parse->command,
				&shell->command_p);
	}
	else
		return (1);
	if (parse->current_option)
		free(parse->current_option);
	return (0);
}

void	pipe_syntax_error(t_shell *shell, t_parse *parse)
{
	write(2, "syntax error near unexpected token `|'\n", 40);
	shell->last_exit_code = 258;
	shell->parse_error = 1;
	free_command(shell);
	shell->command_p = NULL;
	free(parse->start);
}
