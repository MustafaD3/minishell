/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkilic344@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 22:24:27 by mdalkili          #+#    #+#             */
/*   Updated: 2025/07/21 20:51:06 by mdalkili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	find_var_end(const char *str, int start)
{
    int j;
    
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
    expanded = getenv(expand_value) ? ft_strdup(getenv(expand_value)) : ft_strdup("");
    free(expand_value);
    return (expanded);
}

char *expand_if_dollar(const char *str, int *i)
{
    int j;
    
    j = *i + 1;
    if (!(ft_isalnum(str[j]) || str[j] == '_')) {
        (*i)++;
        return ft_strdup("$");
    }
    j = find_var_end(str, j);
    char *result = extract_and_expand_var(str, *i + 1, j);
    *i = j;
    return (result);
}
char *get_next_char(const char *str, int *i)
{
    char *tmp = ft_strndup(str + *i, 1);
    (*i)++;
    return tmp;
}

char *get_characters(char **prompt)
{
    char *result;
    char *tmp;
    char *new_result;
    int i;
    int old_i;

    result = NULL;
    tmp = NULL;
    i = 0;
    while ((*prompt)[i] && (*prompt)[i] != ' ' && (*prompt)[i] != '\'' &&
        (*prompt)[i] != '"')
    {
        old_i = i;
        if ((*prompt)[i] == '$')
            tmp = expand_if_dollar(*prompt, &i);
        else
            tmp = get_next_char(*prompt, &i);
        
        if (i <= old_i) 
            i = old_i + 1;
        if (tmp) {
            new_result = ft_strjoin(result ? result : "", tmp);
            if (result)
                free(result);
            result = new_result;
            free(tmp);
        }
    }
    *prompt += i;
    if (result == NULL)
        return (ft_strdup(""));
    return (result);
}
