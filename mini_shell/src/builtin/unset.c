/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkilic344@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 16:00:35 by makboga           #+#    #+#             */
/*   Updated: 2025/08/04 16:52:19 by mdalkili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int is_valid_identifier(const char *str)
{
    int i = 0;
    if (!str || !str[0])
        return 0;
    if (!(ft_isalpha(str[0]) || str[0] == '_'))
        return 0;
    while (str[++i])
        if (!(ft_isalnum(str[i]) || str[i] == '_'))
            return 0;
    return 1;
}
void unset_env(char ***envp, const char *name)
{
    int i = 0, j = 0, len = ft_strlen(name);
    char **new_envp;

    // Kaç değişken var bul
    while ((*envp)[i])
        i++;
    new_envp = malloc(sizeof(char *) * i);
    if (!new_envp)
        return;
    i = 0;
    while ((*envp)[i])
    {
        if (!(ft_strncmp((*envp)[i], name, len) == 0 && (*envp)[i][len] == '='))
            new_envp[j++] = (*envp)[i];
        else
            free((*envp)[i]);
        i++;
    }
    new_envp[j] = NULL;
    free(*envp);
    *envp = new_envp;
}
int	builtin_unset(t_shell *shell, char **argv)
{
    int i = 1;
    int ret = 0;

    while (argv[i])
    {
        if (!is_valid_identifier(argv[i]))
        {
            ft_putstr_fd("minishell: unset: `", 2);
            ft_putstr_fd(argv[i], 2);
            ft_putstr_fd("': not a valid identifier\n", 2);
            ret = 1;
        }
        else
            unset_env(&(shell->envp), argv[i]);
        i++;
    }
    return ret;
}

