/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkilic344@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:03:40 by mdalkili          #+#    #+#             */
/*   Updated: 2025/08/15 01:29:00 by mdalkili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int prompt_type_control(char **control_list,int type,char *str)
{
	int i;

	if (!str || !*str)
		return (1);
	if (type == 0 && control_list)
	{
		i = 0;
		while (control_list[i])
		{
			if (ft_strcmp(control_list[i], str) == 0)
				return (5);
			i++;
		}
		return (0);
	}
	if (type == 1)
	{
		if (control_list)
		{
			i = 0;
			while (control_list[i])
			{
				if (ft_strcmp(control_list[i], str) == 0)
					return (4);
				i++;
			}
		}
		if (ft_strchr(str, '/'))
			return (3);
		return (2);
	}
	return (0);
}
