/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkilic344@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 07:09:35 by mdalkili          #+#    #+#             */
/*   Updated: 2025/08/23 01:04:52 by mdalkili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	append(t_shell *shell, char *str, int *command, t_command **temp)
{
	char			*temp_str;
	int				result;
	int				token_result;

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
		if (!*command)
			(append_command(shell, "", 0, temp), *command = 1);
		if (*temp)
			append_parameter(temp, str);
	}
	else
		append_parameter(temp, str);
}

void	parse_prompt(t_shell *shell)
{
	t_parse	parse;

	reset_parse(&parse, shell);
	while (parse.temp_prompt && *parse.temp_prompt)
	{
		parse.parse_func = NULL;
		if (*parse.temp_prompt == '\'')
			parse.parse_func = single_quote_control;
		else if (*parse.temp_prompt == '"')
			parse.parse_func = double_quote_control;
		else if (*parse.temp_prompt == '>' || *parse.temp_prompt == '<')
			parse.parse_func = get_redirect_operator;
		else if (*parse.temp_prompt == '|')
		{
			if (parse.command == 0)
				return (pipe_syntax_error(shell, &parse), (void)0);
			parse.had_pipe = 1;
			parse.command = 0;
			parse.command_temp_p = NULL;
			parse.temp_prompt++;
			continue ;
		}
		else if (!ft_isspace(*parse.temp_prompt))
			parse.parse_func = get_characters;
		if (parse.parse_func)
		{
			if (run_parse(&parse, shell))
				break ;
			continue ;
		}
		parse.temp_prompt++;
	}
	if (parse.had_pipe && parse.command == 0)
		return (pipe_syntax_error(shell, &parse), (void)0);
	shell->command_p = parse.command_temp_p;
	(process_redirections(shell), free(parse.start));
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
