/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 16:12:49 by makboga           #+#    #+#             */
/*   Updated: 2025/07/17 16:20:28 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strjoin_3(const char *s1, const char *s2, const char *s3)
{
	char	*res;
	int		len1;
	int		len2;
	int		len3;

	if (!s1 || !s2 || !s3)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	len3 = ft_strlen(s3);
	res = malloc(len1 + len2 + len3 + 1);
	if (!res)
		return (NULL);
	ft_memcpy(res, s1, len1);
	ft_memcpy(res + len1, s2, len2);
	ft_memcpy(res + len1 + len2, s3, len3);
	res[len1 + len2 + len3] = '\0';
	return (res);
}

char	**ft_double_extension(char **matrix, char *new_str)
{
	int		i;
	char	**new_matrix;

	if (!new_str)
		return (matrix);
	i = 0;
	while (matrix && matrix[i])
		i++;
	new_matrix = malloc(sizeof(char *) * (i + 2));
	if (!new_matrix)
		return (NULL);
	i = 0;
	while (matrix && matrix[i])
	{
		new_matrix[i] = matrix[i];
		i++;
	}
	new_matrix[i] = new_str;
	new_matrix[i + 1] = NULL;
	free(matrix);
	return (new_matrix);
}

char	*mini_getenv(const char *key, char **envp)
{
	int		i;
	int		len;

	if (!key || !envp)
		return (NULL);
	len = ft_strlen(key);
	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], key, len) && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

char **mini_setenv(char **envp, const char *key, const char *value, int overwrite)
{
    int i;
    int len;
    char *new_entry;

    len = ft_strlen(key);
    i = 0;
    while (envp && envp[i])
    {
        if (!ft_strncmp(envp[i], key, len) && envp[i][len] == '=')
        {
            if (!overwrite)
                return (envp);
            free(envp[i]);
            new_entry = ft_strjoin_3(key, "=", value);
            envp[i] = new_entry;
            return (envp);
        }
        i++;
    }
    envp = ft_double_extension(envp, ft_strjoin_3(key, "=", value));
    return (envp);
}


char	**mini_unsetenv(char *key, char **envp)
{
	int		i;
	int		j;
	int		len;
	char	**new_env;

	len = ft_strlen(key);
	i = 0;
	j = 0;
	while (envp && envp[i])
		i++;
	new_env = malloc(sizeof(char *) * i); // bir eksik çünkü biri silinecek
	i = 0;
	while (envp && envp[i])
	{
		if (!ft_strncmp(envp[i], key, len) && envp[i][len] == '=')
			free(envp[i]);
		else
			new_env[j++] = envp[i];
		i++;
	}
	new_env[j] = NULL;
	free(envp);
	return (new_env);
}
