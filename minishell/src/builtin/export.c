/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:53:24 by makboga           #+#    #+#             */
/*   Updated: 2025/08/14 13:38:32 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

static void	print_export_vars(char **envp)
{
	int		i;
	char	*marker_pos;
	char	*clean_entry;

	i = 0;
	while (envp[i])
	{
		marker_pos = ft_strstr(envp[i], "=__EXPORT_ONLY__");
		if (marker_pos)
		{
			clean_entry = ft_substr(envp[i], 0, marker_pos - envp[i]);
			printf("declare -x %s\n", clean_entry);
			free(clean_entry);
		}
		else
			printf("declare -x %s\n", envp[i]);
		i++;
	}
}

int	builtin_export(char ***envp, char **argv)
{
	int	i;
	int	err;

	if (!argv[1])
	{
		print_export_vars(*envp);
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
