/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkilic344@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 20:07:25 by mdalkili          #+#    #+#             */
/*   Updated: 2025/08/21 20:46:58 by mdalkili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*dq_expand_and_concat(const char *str, int start, int end,
	t_shell *shell)
{
	char	*res;
	char	*tmp;
	int		i;

	res = ft_strdup("");
	if (!res)
		return (NULL);
	i = start;
	while (i <= end)
	{
		if (str[i] == '\"')
			i++;
		else
		{
			if (str[i] == '$')
				tmp = expand_if_dollar(str, &i, shell);
			else
				tmp = get_next_char(str, &i);
			if (!tmp)
				return (res);
			res = ft_strjoin_free(res, tmp);
			free(tmp);
		}
	}
	return (res);
}

static char	*double_quote(char **prompt, t_shell *shell)
{
	char	*start;
	char	*end;
	char	*result;

	start = *prompt;
	end = ft_strchr(start + 1, '"');
	if (!end)
	{
		write(STDERR_FILENO, "Syntax error: missing closing quote\n", 37);
		shell->last_exit_code = 258;
		*prompt += 1;
		return (NULL);
	}
	else
	{
		result = dq_expand_and_concat(start, 1, end - start, shell);
		*prompt = end + 1;
		return (result);
	}
}

void	re_call(t_shell *shell, char**result,
	char **prompt, char *func(char **, t_shell *))
{
	char	*temp;

	temp = func(prompt, shell);
	*result = set_and_free(*result, ft_strjoin(*result, temp));
	free(temp);
}

char	*double_quote_control(char **prompt, t_shell *shell)
{
	char	*result;

	shell->is_quote = 1;
	result = double_quote(prompt, shell);
	while (**prompt && **prompt != ' ')
	{
		if (**prompt == '"' && *(*prompt + 1) != '"')
		{
			shell->is_quote = 1;
			re_call(shell, &result, prompt, double_quote_control);
		}
		else if (**prompt == '\'' && *(*prompt + 1) != '\'')
		{
			shell->is_quote = 1;
			re_call(shell, &result, prompt, single_quote_control);
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
