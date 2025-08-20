/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkilic344@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 10:36:58 by mdalkili          #+#    #+#             */
/*   Updated: 2025/08/19 02:53:55 by mdalkili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*single_quote(char **prompt, t_shell *shell)
{
	char	*start;
	char	*end;

	start = *prompt;
	end = ft_strchr(start + 1, '\'');
	if (!end)
	{
		write(STDERR_FILENO, "Syntax error: missing closing quote\n", 37);
		shell->last_exit_code = 258;
		*prompt += 1;
		return (NULL);
	}
	else
	{
		*prompt = end + 1;
		return (ft_strndup(start + 1, end - start - 1));
	}
}

char	*single_quote_control(char **prompt, t_shell *shell)
{
	char	*result;

	shell->is_quote = 1;
	result = single_quote(prompt, shell);
	while (**prompt && **prompt != ' ')
	{
		if (**prompt == '\'' && *(*prompt + 1) != '\'')
		{
			shell->is_quote = 1;
			re_call(shell, &result, prompt, single_quote_control);
		}
		else if (**prompt == '"' && *(*prompt + 1) != '"')
		{
			shell->is_quote = 1;
			re_call(shell, &result, prompt, double_quote_control);
		}
		else if (**prompt == '"' && *(*prompt + 1) == '"')
			*prompt += 2;
		else if (**prompt == '\'' && *(*prompt + 1) == '\'')
			*prompt += 2;
		else
			re_call(shell, &result, prompt, get_characters);
	}
	return (result);
}
