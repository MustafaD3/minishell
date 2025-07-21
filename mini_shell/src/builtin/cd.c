/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 16:08:25 by makboga           #+#    #+#             */
/*   Updated: 2025/07/17 16:23:01 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	builtin_cd(t_shell *shell, char **args)
{
	char	*oldpwd;
	char	*newpwd;

	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		return (write(2, "cd: error getting current directory\n", 36), 1);

	if (!args[1] || !args[1][0])
		newpwd = mini_getenv("HOME", shell->envp);
	else
		newpwd = args[1];

	if (!newpwd || chdir(newpwd) != 0)
	{
		free(oldpwd);
		write(2, "cd: No such file or directory\n", 30);
		return (1);
	}

	shell->envp = mini_setenv(shell->envp, "OLDPWD", oldpwd, 1);
	free(oldpwd);

	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		return (write(2, "cd: error getting current directory\n", 36), 1);

	shell->envp = mini_setenv(shell->envp, "PWD", oldpwd, 1);
	free(oldpwd);

	return (0);
}

