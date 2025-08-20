/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 13:40:00 by makboga           #+#    #+#             */
/*   Updated: 2025/08/14 19:22:31 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_valid_identifier(const char *str)
{
	int	i;

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

int	update_env(char ***envp, const char *key, const char *value)
{
	int		i;
	int		key_len;
	char	*new_entry;
	char	*temp;

	i = 0;
	key_len = ft_strlen(key);
	while ((*envp)[i])
	{
		if (!ft_strncmp((*envp)[i], key, key_len) && (*envp)[i][key_len] == '=')
		{
			free((*envp)[i]);
			temp = ft_strjoin(key, "=");
			new_entry = ft_strjoin(temp, value);
			free(temp);
			if (!new_entry)
				return (0);
			(*envp)[i] = new_entry;
			return (1);
		}
		i++;
	}
	return (0);
}

void	add_env(char ***envp, const char *key_value)
{
	int		i;
	int		size;
	char	**new_envp;

	i = 0;
	size = 0;
	while ((*envp)[size])
		size++;
	new_envp = malloc(sizeof(char *) * (size + 2));
	if (!new_envp)
		return ;
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

int	print_invalid_identifier(char *arg)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (1);
}

int	export_single_var_helper(char ***envp, char *key, char *arg,
	char *equal_pos)
{
	char	*export_entry;

	if (equal_pos)
	{
		if (!update_env(envp, key, equal_pos + 1))
			add_env(envp, arg);
	}
	else
	{
		export_entry = ft_strjoin(key, "=__EXPORT_ONLY__");
		if (export_entry)
		{
			add_env(envp, export_entry);
			free(export_entry);
		}
	}
	return (0);
}
