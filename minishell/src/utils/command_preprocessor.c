/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_preprocessor.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 18:00:00 by makboga           #+#    #+#             */
/*   Updated: 2025/08/17 16:39:49 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*normalize_redirections(char *command)
{
	if (!command)
		return (NULL);
	while (*command == ' ' || *command == '\t')
		command++;
	if (*command == '>' || *command == '<')
		return (ft_strdup(command));
	return (ft_strdup(command));
}
