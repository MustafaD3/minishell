/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkilic344@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 20:07:25 by mdalkili          #+#    #+#             */
/*   Updated: 2025/07/18 17:06:54 by mdalkili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char *dq_expand_and_concat(const char *str, int start, int end)
{
    char *result;
    char *tmp;
    int i;
    
    result = NULL;
    i = start;
    while (i < end) {
        if (str[i] == '$')
            tmp = expand_if_dollar(str, &i);
        else
            tmp = get_next_char(str, &i);
        result = set_and_free(result, ft_strjoin(result ? result : "", tmp));
        free(tmp);
    }
    return result;
}

static void double_quote_loop(t_quote *quote)
{
	char *expanded;
	
	quote->current_parameter = set_and_free(quote->current_parameter, readline("> "));
    while(quote->current_parameter)
    {
        quote->len++;
        if(ft_strchr(quote->current_parameter, '"') && counter_quote(quote->current_parameter, "\"") % 2 == 1)
        {
            expanded = dq_expand_and_concat(quote->current_parameter, 0, ft_strlen(quote->current_parameter));
            quote->parameters = copy_multiple_input(quote->parameters, expanded, quote->len);
            free(expanded);
            break;
        }
        expanded = dq_expand_and_concat(quote->current_parameter, 0, ft_strlen(quote->current_parameter));
        quote->parameters = copy_multiple_input(quote->parameters, expanded, quote->len);
        free(expanded);
        quote->parameters = copy_multiple_input(quote->parameters, "\n", ++quote->len);
        quote->current_parameter = set_and_free(quote->current_parameter, readline("> "));
    }
}

static char *double_quote(char **prompt)
{
	char	*start;
	char	*end;
	char	*result;
	char	*expanded_first;
	t_quote *quote;

	quote = quote_init();
	start = *prompt;
	end = ft_strchr(start + 1, '"');
	if (!end)
	{
		expanded_first = dq_expand_and_concat(start + 1, 0, ft_strlen(start + 1));
		quote->parameters = copy_multiple_input(quote->parameters, expanded_first, ++quote->len);
		free(expanded_first);
		quote->parameters = copy_multiple_input(quote->parameters, "\n", ++quote->len);
		double_quote_loop(quote);
	}
	if(quote->len > 0)
	{
		result = string_concatation_heap(quote->parameters);
		result[ft_strlen(result) - 1] = '\0';
		*prompt = start + ft_strlen(start);
		free_quote(quote);
		return result;
	}
	else{
		result = dq_expand_and_concat(start, 1, end - start);
		*prompt = end + 1;
		free_quote(quote);
		return result;
	}
}
char *double_quote_control(char **prompt)
{
	char *result;
	char *temp;

	result = double_quote(prompt);
	while(**prompt && **prompt != '\'' && **prompt != ' ')
	{
		if(**prompt == '"')
		{
			temp = double_quote(prompt);
			result = set_and_free(result, ft_strjoin(result, temp));
			free(temp);
		}
		else{
			temp = get_characters(prompt);
			result = set_and_free(result, ft_strjoin(result, temp));
			free(temp);
		}
	}
	return result;
}
