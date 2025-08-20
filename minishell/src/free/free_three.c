/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_three.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkilic344@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:59:38 by makboga           #+#    #+#             */
/*   Updated: 2025/08/19 00:34:34 by mdalkili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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
