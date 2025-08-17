/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 10:36:58 by mdalkili          #+#    #+#             */
/*   Updated: 2025/08/17 18:14:13 by makboga          ###   ########.fr       */
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

static void single_quote_loop(t_quote *quote)
{
	quote->current_parameter = set_and_free(quote->current_parameter, readline("> "));
	while(quote->current_parameter)
	{
		quote->len++;
		if(ft_strchr(quote->current_parameter, '\'') && counter_quote(quote->current_parameter, "'") % 2 == 1)
		{
			quote->parameters = copy_multiple_input(quote->parameters, quote->current_parameter, quote->len);
			break;
		}
		quote->parameters = copy_multiple_input(quote->parameters, quote->current_parameter, quote->len);
		quote->parameters = copy_multiple_input(quote->parameters, "\n", ++quote->len);
		quote->current_parameter = set_and_free(quote->current_parameter, readline("> "));
	}
}

static char *single_quote(char **prompt)
{
	char	*start;
	char	*end;
	char	*result;
	t_quote *quote;

	quote = quote_init();
	start = *prompt;
	end = ft_strchr(start + 1, '\'');
	if (!end)
	{
		quote->parameters = copy_multiple_input(quote->parameters, start + 1, ++quote->len);
		quote->parameters = copy_multiple_input(quote->parameters, "\n", ++quote->len);
		single_quote_loop(quote);
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
		*prompt = end + 1;
		free_quote(quote);
		return ft_strndup(start + 1, end - start - 1);
	}
}
char *single_quote_control(char **prompt,t_shell *shell)
{
	char *result;
	char *temp;

	result = single_quote(prompt);
	while(**prompt &&  **prompt != '"' && **prompt != ' ')
	{
		if(**prompt == '\'')
		{
			temp = single_quote(prompt);
			char *new_result = ft_strjoin(result ? result : "", temp);
			free(result);
			free(temp);
			result = new_result;
		}
		else
		{
			temp = get_characters(prompt,shell);
			char *new_result = ft_strjoin(result ? result : "", temp);
			free(result);
			free(temp);
			result = new_result;
		}
	}
	if(**prompt == '"' && *(*prompt + 1) != '"')
	{
		temp = result;
		char *double_quote_result = double_quote_control(prompt,shell);
		result = ft_strjoin(temp ? temp : "", double_quote_result);
		if (temp)
			free(temp);
		if (double_quote_result)
			free(double_quote_result);
	}
	return result;
}
