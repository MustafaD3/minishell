/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkilic344@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:03:40 by mdalkili          #+#    #+#             */
/*   Updated: 2025/08/19 02:54:59 by mdalkili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	prompt_type_control_loop(char **control_list, int type, char *str)
{
	int	i;

	i = 0;
	if (type)
	{
		while (control_list && control_list[i])
		{
			if (ft_strcmp(control_list[i++], str) == 0)
				return (3);
		}
		if (access(str, F_OK) == 0)
			return (2);
		else
			return (1);
	}
	else
	{
		while (control_list[i])
		{
			if (ft_strcmp(control_list[i], str) == 0)
				return (4);
			i++;
		}
	}
	return (0);
}
