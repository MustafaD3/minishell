/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkilic344@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:03:40 by mdalkili          #+#    #+#             */
/*   Updated: 2025/07/22 02:03:55 by mdalkili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void free_multiple_input(char **multiple_input)
{
	int i = 0;
	while (multiple_input && multiple_input[i])
	{
		free(multiple_input[i]);
		i++;
	}
	free(multiple_input);
}
void print_parameters(t_quote *quote)
{
	int	i;
	int	j;
	
	i = 0;
	j = 0;
	while (quote->parameters && quote->parameters[i])
	{
		printf("\n");
		j = 0;
		while(quote->parameters[i][j])
			if(quote->parameters[i][j] != '\'')
				write(STDOUT_FILENO, &quote->parameters[i][j++], 1);
			else
				j++;
		i++;
	}
}

char **copy_multiple_input(char **multiple_input,char *temp, int len)
{
	char	**new_multiple_input;
	int		i;
	
	i = 0;
	new_multiple_input = malloc(sizeof(char *) * (len +  1));
	while (multiple_input && multiple_input[i])
	{
		new_multiple_input[i] = ft_strdup(multiple_input[i]);
		i++;
	}
	new_multiple_input[i++] = ft_strdup(temp);
	new_multiple_input[i] = NULL;
	free_multiple_input(multiple_input);
	return (new_multiple_input);
}
int counter_quote(char *str, char *quoter)
{
	int i;
	int count;

	i = 0;
	count = 0;
	while (str && str[i])
	{
		if (str[i] == *quoter)
			count++;
		i++;
	}
	return (count);
}
t_quote *quote_init(void)
{
	t_quote *quote;

	quote = malloc(sizeof(t_quote));
	if (!quote)
		exit_with_error("Error initializing quote structure");
	quote->parameters = NULL;
	quote->current_parameter = NULL;
	quote->flag = 0;
	quote->len = 0;
	return (quote);
}
