/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 22:24:27 by mdalkili          #+#    #+#             */
/*   Updated: 2025/08/08 18:09:39 by mdalkili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// Post-expansion token checking function
char *check_post_expansion_tokens(char *expanded_str, t_shell *shell)
{
    // Sadece token'ları kontrol et, diğer logic'i bozmayalım
    if (!expanded_str || !shell)
        return expanded_str;
    
    // Basit token kontrolü - sadece temel redirection token'ları
    if (ft_strcmp(expanded_str, ">>") == 0 || 
        ft_strcmp(expanded_str, "<<") == 0 || 
        ft_strcmp(expanded_str, ">") == 0 || 
        ft_strcmp(expanded_str, "<") == 0 || 
        ft_strcmp(expanded_str, "|") == 0)
    {
        // Token olarak işaretle (özel bir prefix ekle)
        char *token_result = ft_strjoin("__TOKEN__", expanded_str);
        free(expanded_str);
        return token_result;
    }
    
    return expanded_str;
}

static int	find_var_end(const char *str, int start)
{
    int j;
    
    j = start;
    while (str[j] && (ft_isalnum(str[j]) || str[j] == '_'))
        j++;
    return (j);
}

static char	*extract_and_expand_var(const char *str, int start, int end, t_shell *shell)
{
    char	*expand_value;
    char	*expanded;
    
    expand_value = ft_strndup(str + start, end - start);
    if (!expand_value)
        return (ft_strdup(""));
    
    // Use minishell's environment instead of system getenv
    expanded = mini_getenv(expand_value, shell->envp);
    if (expanded)
        expanded = ft_strdup(expanded);
    else
        expanded = ft_strdup("");
        
    free(expand_value);
    return (expanded);
}

char *expand_if_dollar(const char *str, int *i,t_shell *shell)
{
    int j;
    
    j = *i + 1;
	if (str[*i + 1] == '?')
    {
        *i += 2;
        return ft_itoa(shell->last_exit_code);
    }
    if (!(ft_isalnum(str[j]) || str[j] == '_')) {
        (*i)++;
        return ft_strdup("$");
    }
    j = find_var_end(str, j);
    char *result = extract_and_expand_var(str, *i + 1, j, shell);
    *i = j;
    return (result);
}
char *get_next_char(const char *str, int *i)
{
    char *tmp = ft_strndup(str + *i, 1);
    (*i)++;
    return tmp;
}

char *get_characters(char **prompt,t_shell *shell)
{
    char *result;
    char *tmp;
    char *new_result;
    int i;
    int old_i;

    result = NULL;
    tmp = NULL;
    i = 0;
    while ((*prompt)[i] && (*prompt)[i] != ' ' && (*prompt)[i] != '\t' && 
        (*prompt)[i] != '\'' && (*prompt)[i] != '"')
    {
        old_i = i;
        if ((*prompt)[i] == '$')
            tmp = expand_if_dollar(*prompt, &i,shell);
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
	if(**prompt == '\'' && *(*prompt + 1) != '\'')
	{
		tmp = result;
		result = ft_strjoin(tmp,single_quote_control(prompt,shell));
		if (tmp)
			free(tmp);
	}
	else if(**prompt == '"' && *(*prompt + 1) != '"')
	{
		tmp = result;
		result = ft_strjoin(tmp,double_quote_control(prompt,shell));
		if (tmp)
			free(tmp);
	}
    if (result == NULL)
        return (ft_strdup(""));
    
    // Post-expansion token check - sadece unquoted expansion için
    result = check_post_expansion_tokens(result, shell);
    
    return (result);
}
