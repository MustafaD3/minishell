/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkilic344@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:38:48 by makboga           #+#    #+#             */
/*   Updated: 2025/08/04 19:19:54 by mdalkili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	is_valid_option(const char *param)
{
    int	i;

    if (!param || param[0] != '-' || param[1] == '\0')
        return (0);
    i = 1;
    while (param[i])
    {
        if (param[i] != 'L' && param[i] != 'P')
            return (0);
        i++;
    }
    return (1);
}

int	builtin_pwd(char **params)
{
	char	*cwd;
	char	*pwd_env;
	int		use_logical;
	int		i;

	use_logical = 1;
	i = 1;
	while (params && params[i])
	{
		if (is_valid_option(params[i]))
		{
			if (ft_strchr(params[i], 'P'))
				use_logical = 0;
		}
		else
			write(STDERR_FILENO,"pwd: %s: seçenek olmayan argümanlar yok sayılıyor\n", 38);
		i++;
	}
	pwd_env = getenv("PWD");
	if (use_logical && pwd_env && access(pwd_env, F_OK) == 0)
		return (printf("%s\n", pwd_env), 0);
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (write(STDERR_FILENO,"pwd: failed to get current directory\n", 38), 1);
	return (printf("%s\n", cwd), free(cwd), 0);
}
