/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 16:08:25 by makboga           #+#    #+#             */
/*   Updated: 2025/08/14 13:25:08 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	update_pwd_vars(t_shell *shell, char *oldpwd)
{
	char	*newpwd;

	newpwd = getcwd(NULL, 0);
	if (!newpwd)
		return (write(2, "cd: error getting current directory\n", 36), 1);
	shell->envp = mini_setenv(shell->envp, "OLDPWD", oldpwd, 1);
	shell->envp = mini_setenv(shell->envp, "PWD", newpwd, 1);
	free(newpwd);
	return (0);
}

int	builtin_cd(t_shell *shell, char **args)
{
	char	*oldpwd;
	char	*newpwd;

	if (args[1] && args[2])
		return (write(2, "cd: too many arguments\n", 23), 1);
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
		return (write(2, "cd: No such file or directory\n", 30), 1);
	}
	update_pwd_vars(shell, oldpwd);
	free(oldpwd);
	return (0);
}
