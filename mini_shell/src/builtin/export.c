/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkilic344@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:53:24 by makboga           #+#    #+#             */
/*   Updated: 2025/08/04 19:17:37 by mdalkili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	is_valid_identifier(const char *str)
{
	int i;

	if (!str || !*str)
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	update_env(char ***envp, const char *key, const char *value)
{
	int i = 0;
	int key_len = ft_strlen(key);
	char *new_entry;

	while ((*envp)[i])
	{
		if (!ft_strncmp((*envp)[i], key, key_len) && (*envp)[i][key_len] == '=')
		{
			free((*envp)[i]);
			new_entry = ft_strjoin_3(key, "=", value);
			if (!new_entry)
				return (0);
			(*envp)[i] = new_entry;
			return (1);
		}
		i++;
	}
	return (0);
}

static void	add_env(char ***envp, const char *key_value)
{
	int i = 0;
	int size = 0;
	char **new_envp;

	while ((*envp)[size])
		size++;
	new_envp = malloc(sizeof(char *) * (size + 2));
	if (!new_envp)
		return;
	while (i < size)
	{
		new_envp[i] = (*envp)[i];
		i++;
	}
	new_envp[i++] = ft_strdup(key_value);
	new_envp[i] = NULL;
	free(*envp);
	*envp = new_envp;
}

static int	print_invalid_identifier(char *arg)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (1);
}

static int	export_single_var_helper(char ***envp, char *key, char *arg, char *equal_pos)
{
	if (equal_pos)
	{
		if (!update_env(envp, key, equal_pos + 1))
			add_env(envp, arg);
	}
	else
	{
		if (!update_env(envp, key, ""))
			add_env(envp, arg);
	}
	return (0);
}

static int	export_single_var(char ***envp, char *arg)
{
	char	*equal_pos;
	char	*key;
	int		ret;

	equal_pos = ft_strchr(arg, '=');
	if (equal_pos)
		key = ft_substr(arg, 0, equal_pos - arg);
	else
		key = ft_strdup(arg);
	if (!key)
		return (1);
	if (!is_valid_identifier(key))
	{
		free(key);
		return (print_invalid_identifier(arg));
	}
	ret = export_single_var_helper(envp, key, arg, equal_pos);
	free(key);
	return (ret);
}

void	print_exported_vars(char **envp)
{
    int		i, j, count = 0;
    char	**sorted, *eq;

    while (envp[count])
        count++;
    sorted = malloc(sizeof(char *) * (count + 1));
    if (!sorted)
        return;
    for (i = 0; i < count; i++)
        sorted[i] = envp[i];
    sorted[count] = NULL;
    for (i = 0; i < count - 1; i++)
        for (j = 0; j < count - i - 1; j++)
            if (ft_strcmp(sorted[j], sorted[j + 1]) > 0)
            {
                char *tmp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = tmp;
            }
    for (i = 0; i < count; i++)
    {
        eq = ft_strchr(sorted[i], '=');
        if (eq)
		//copilot printfleri (write fonksiyonu ile değiştir dprintf ile değil Sayılar yerine stdoutfileno errfileno değerlerini kullan)
            printf("declare -x %.*s=\"%s\"\n", (int)(eq - sorted[i]), sorted[i], eq + 1);
        else
            printf("declare -x %s\n", sorted[i]);
    }
    free(sorted);
}

int	builtin_export(char ***envp, char **argv)
{
    int	i;
    int err;

    // Argümansız export durumu eklendi
    if (!argv[1])
    {
        print_exported_vars(*envp);
        return (0);
    }

    i = 1;
    err = 0;
    while (argv[i])
    {
        if (export_single_var(envp, argv[i]) != 0)
            err = 1;
        i++;
    }
    return (err);
}
