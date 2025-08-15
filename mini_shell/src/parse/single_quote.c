/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkilic344@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 10:36:58 by mdalkili          #+#    #+#             */
/*   Updated: 2025/08/15 04:26:31 by mdalkili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*string_concatation_heap(char **str)
{
	char	*result;
	int		total_byte;
	int		i;
	
	i = 0;
	total_byte = 0;
	while(str[i]){
		total_byte+= ft_strlen(str[i]);
		i++;
	}
	result = malloc(total_byte + 1);
	i = 0;
	total_byte = 0;
	if(!result)
		return (NULL);
	while(str[i]){
		ft_memcpy(result + total_byte, str[i], ft_strlen(str[i]));
		total_byte += ft_strlen(str[i]);
		i++;
	}
	ft_memcpy(result + total_byte, "\0", 1);
	return (result);
}

static char *single_quote(char **prompt, t_shell *shell)
{
	char	*start;
	char	*end;
	
	start = *prompt;
	end = ft_strchr(start + 1, '\'');
	if (!end)
	{
		write(STDERR_FILENO,"Syntax error: missing closing single quote\n", 42);
		shell->last_exit_code = 258;
		*prompt += 1;
		return NULL;
	}
	else{
		*prompt = end + 1;
		return ft_strndup(start + 1, end - start - 1);
	}
}
char *single_quote_control(char **prompt,t_shell *shell)
{
	char *result;
	char *temp;

	shell->is_quote = 1;
	result = single_quote(prompt, shell);
	while(**prompt &&  **prompt != '"' && **prompt != ' ')
	{
		if(**prompt == '\'' && *(*prompt + 1) != '\'')
		{
			temp = single_quote(prompt, shell);
			result = set_and_free(result, ft_strjoin(result, temp));
			free(temp);
		}
		else if(**prompt == '"' && *(*prompt + 1) != '"')
		{
			temp = double_quote_control(prompt,shell);
			result = ft_strjoin(result, temp);
			if (temp)
				free(temp);
		}
		else if (**prompt == '\'' && *(*prompt + 1) == '\'')
			*prompt += 2;
		else if (**prompt == '"' && *(*prompt + 1) == '"')
			*prompt += 2;
		else
		{
			temp = get_characters(prompt,shell);
			result = set_and_free(result, ft_strjoin(result, temp));
			free(temp);
		}
	}
	return result;
}
