/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ismeta.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 15:36:19 by makboga           #+#    #+#             */
/*   Updated: 2025/08/16 15:36:19 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

bool	ft_ismeta(char *str, int i)
{
	int		j;
	char	*metachars;

	j = 0;
	metachars = "<|>";
	if (!str || i < 0)
		return (false);
	while (metachars[j] && str[i])
	{
		if (str[i] && str[i] == metachars[j])
			return (true);
		j++;
	}
	return (false);
}
