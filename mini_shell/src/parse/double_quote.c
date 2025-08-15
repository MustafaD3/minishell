/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkilic344@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 20:07:25 by mdalkili          #+#    #+#             */
/*   Updated: 2025/08/15 03:47:10 by mdalkili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char *dq_expand_and_concat(const char *str, int start, int end,t_shell *shell)
{
    char *result;
    char *tmp;
    int i;

	shell->is_quote = 1;
    result = NULL;
    i = start;
    while (i <= end ) {
		if(str[i] == '"')
		{
			i++;
			continue;
		}
        if (str[i] == '$')
            tmp = expand_if_dollar(str, &i,shell);
        else
            tmp = get_next_char(str, &i);
		if(result)
			result = set_and_free(result, ft_strjoin(result, tmp));
        else
			result = set_and_free(result, ft_strdup(tmp));
        free(tmp);
    }
    return result;
}

static char *double_quote(char **prompt,t_shell *shell)
{
	char	*start;
	char	*end;
	char	*result;

	start = *prompt;
	end = ft_strchr(start + 1, '"');
	if (!end)
	{
		write(STDERR_FILENO,"Syntax error: missing closing double quote\n", 42);
		shell->last_exit_code = 258;
		*prompt += 1;
		return NULL;
	}
	else{
		result = dq_expand_and_concat(start, 1, end - start,shell);
		*prompt = end + 1;
		return result;
	}
}
char *double_quote_control(char **prompt,t_shell *shell)
{
	char *result;
	char *temp;

	result = double_quote(prompt,shell);
	while(**prompt && **prompt != ' ')
	{
		if(**prompt == '"' && *(*prompt + 1) != '"')
		{
			shell->is_quote = 1;
			temp = double_quote(prompt,shell);
			result = set_and_free(result, ft_strjoin(result, temp));
			free(temp);
		}
		else if(**prompt == '\'' && *(*prompt + 1) != '\'')
		{
			shell->is_quote = 1;
			temp = single_quote_control(prompt, shell);
			result = set_and_free(result,ft_strjoin(temp,result));
			if (temp)
				free(temp);
		}
		else if (**prompt == '"' && *(*prompt + 1) == '"')
			*prompt += 2;
		else if (**prompt == '\'' && *(*prompt + 1) == '\'')
			*prompt += 2;
		else{
			shell->is_quote = 0;
			temp = get_characters(prompt,shell);
			result = set_and_free(result, ft_strjoin(result, temp));
			free(temp);
		}
	}
	
	return result;
}
