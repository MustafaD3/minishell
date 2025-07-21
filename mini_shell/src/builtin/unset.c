/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 16:00:35 by makboga           #+#    #+#             */
/*   Updated: 2025/07/17 16:37:11 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	var_name_match(const char *env_var, const char *name)
{
	int	i = 0;
	while (env_var[i] && name[i] && env_var[i] != '=')
	{
		if (env_var[i] != name[i])
			return (0);
		i++;
	}
	return (name[i] == '\0' && env_var[i] == '=');
}

static char	**remove_env_var(char **envp, const char *name)
{
	int		i;
	int		j;
	int		count;
	char	**new_env;

	count = 0;
	while (envp[count])
		count++;
	new_env = malloc(sizeof(char *) * count);
	if (!new_env)
		return (NULL);
	i = 0;
	j = 0;
	while (envp[i])
	{
		if (!var_name_match(envp[i], name))
		{
			new_env[j] = ft_strdup(envp[i]);
			if (!new_env[j])
			{
				while (j > 0)
					free(new_env[--j]);
				free(new_env);
				return (NULL);
			}
			j++;
		}
		i++;
	}
	new_env[j] = NULL;
	return (new_env);
}

int	builtin_unset(t_shell *shell, char *name)
{
	char	**new_env;

	if (!name || !*name)
		return (1);
	new_env = remove_env_var(shell->envp, name);
	if (!new_env)
		return (1);

	ft_free_matrix(shell->envp);
	shell->envp = new_env;
	return (0);
}

