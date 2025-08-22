/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkilic344@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 22:24:27 by mdalkili          #+#    #+#             */
/*   Updated: 2025/08/23 01:05:59 by mdalkili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	find_var_end(const char *str, int start)
{
	int	j;

	j = start;
	while (str[j] && (ft_isalnum(str[j]) || str[j] == '_'))
		j++;
	return (j);
}

static char	*extract_and_expand_var(const char *str, int start, int end)
{
	char	*expand_value;
	char	*expanded;

	expand_value = ft_strndup(str + start, end - start);
	if (!expand_value)
		return (ft_strdup(""));
	if (getenv(expand_value))
		expanded = ft_strdup(getenv(expand_value));
	else
		expanded = ft_strdup("");
	free(expand_value);
	return (expanded);
}

char	*expand_if_dollar(const char *str, int *i, t_shell *shell)
{
	int		j;
	char	*result;

	j = *i + 1;
	if (str[*i + 1] == '?')
	{
		(*i) += 2;
		return (ft_itoa(shell->last_exit_code));
	}
	if (str[*i + 1] == '"')
	{
		(*i)++;
		return (ft_strdup("$"));
	}
	if (!(ft_isalnum(str[j]) || str[j] == '_'))
	{
		(*i)++;
		return (ft_strdup("$"));
	}
	j = find_var_end(str, j);
	result = extract_and_expand_var(str, *i + 1, j);
	*i = j;
	return (result);
}

char	*get_next_char(const char *str, int *i)
{
	char	*tmp;

	tmp = ft_strndup(str + *i, 1);
	(*i)++;
	return (tmp);
}

char	*get_characters(char **prompt, t_shell *shell)
{
	char	*result;
	char	*tmp;
	char	*new_result;
	int		i;
	int		old_i;

	result = NULL;
	tmp = NULL;
	i = 0;
	while ((*prompt)[i] && !ft_isspace((*prompt)[i]) &&
		(*prompt)[i] != '\'' && (*prompt)[i] != '"')
	{
		old_i = i;
		if ((*prompt)[i] == '$')
			tmp = expand_if_dollar(*prompt, &i, shell);
		else
			tmp = get_next_char(*prompt, &i);
		if (i <= old_i)
			i = old_i + 1;
		if (tmp)
		{
			new_result = ft_strjoin(result ? result : "", tmp);
			if (result)
				free(result);
			result = new_result;
			free(tmp);
		}
	}
	*prompt += i;
	return (re_control(tmp, result, prompt, shell));
}

char	*get_redirect_operator(char **prompt, t_shell *shell)
{
	char	*result;

	(void)shell;
	if (!*prompt || !**prompt)
		return (NULL);
	if (**prompt == '>' && *(*prompt + 1) == '>')
	{
		result = ft_strdup(">>");
		*prompt += 2;
		return (result);
	}
	else if (**prompt == '<' && *(*prompt + 1) == '<')
	{
		result = ft_strdup("<<");
		*prompt += 2;
		return (result);
	}
	else if (**prompt == '>')
	{
		result = ft_strdup(">");
		*prompt += 1;
		return (result);
	}
	else if (**prompt == '<')
	{
		result = ft_strdup("<");
		*prompt += 1;
		return (result);
	}
	return (NULL);
}
