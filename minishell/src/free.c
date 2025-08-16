/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:23:54 by makboga           #+#    #+#             */
/*   Updated: 2025/08/16 16:32:28 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_argv(char **argv)
{
	int	j;

	j = 0;
	while (argv[j])
	{
		free(argv[j]);
		j++;
	}
	free(argv);
}

void	ft_free_split(char **split)
{
	int	i;

	i = 0;
	if (!split)
		return ;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

void	free_shell(t_shell *shell)
{
	int	i;

	if (!shell)
		return ;
	if (shell->prompt)
		free(shell->prompt);
	if (shell->current_dir)
		free(shell->current_dir);
	if (shell->display_info)
		free(shell->display_info);
	if (shell->hostname)
		free(shell->hostname);
	i = 0;
	while (shell->builtin[i])
	{
		free(shell->builtin[i]);
		i++;
	}
	i = 0;
	while (shell->tokens[i])
	{
		free(shell->tokens[i]);
		i++;
	}
	free_command(shell);
	if (shell->envp)
	{
		i = 0;
		while (shell->envp[i])
		{
			free(shell->envp[i]);
			i++;
		}
		free(shell->envp);
	}
}

void	ft_free_matrix(char **matrix)
{
	int	i;

	i = 0;
	while (matrix && matrix[i])
		free(matrix[i++]);
	free(matrix);
}

char	*ft_strjoin_free(char *s1, const char *s2)
{
	char	*result;
	size_t	len1;
	size_t	len2;

	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	result = malloc(len1 + len2 + 1);
	if (!result)
	{
		free(s1);
		return (NULL);
	}
	ft_strlcpy(result, s1, len1 + 1);
	ft_strlcat(result, s2, len1 + len2 + 1);
	free(s1);
	return (result);
}

char	*set_and_free(char *dest, char *src)
{
	if (dest)
		free(dest);
	if (!src)
		return (NULL);
	return (src);
}

void	free_options(t_shell *shell)
{
	int	i;

	i = 0;
	if (shell->options)
	{
		while (shell->options[i])
		{
			free(shell->options[i]);
			i++;
		}
		free(shell->options);
	}
}

void	free_quote(t_quote *quote)
{
	if (!quote)
		return ;
	free(quote->current_parameter);
	free_multiple_input(quote->parameters);
	free(quote);
}

void	free_command(t_shell *shell)
{
	t_command		*current;
	t_command		*next;
	t_parameters	*param;
	t_parameters	*next_param;

	if (shell->command_p)
	{
		current = shell->command_p;
		while (current)
		{
			next = current->next;
			if (current->parameters_p)
			{
				param = current->parameters_p;
				while (param)
				{
					next_param = param->next;
					free(param->parameter);
					free(param);
					param = next_param;
				}
			}
			if (current->redirections)
				free_redirections(current->redirections);
			free(current->command);
			if (current->token)
				free(current->token);
			free(current);
			current = next;
		}
	}
}

void	free_parameters(t_parameters *params)
{
	t_parameters	*temp;

	while (params)
	{
		temp = params;
		params = params->next;
		free(temp->parameter);
		free(temp);
	}
}
