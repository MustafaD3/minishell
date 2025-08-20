/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 15:39:07 by makboga           #+#    #+#             */
/*   Updated: 2025/08/16 15:41:30 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strstr(const char *str, const char *find)
{
	size_t	i;
	size_t	j;

	j = 0;
	i = 0;
	if (!str)
		return ((char *)str);
	if (!(*find))
		return ((char *)str);
	while (str[i])
	{
		j = 0;
		while (str[i] == find[j] && str[i])
		{
			i++;
			j++;
		}
		if (!find[j])
			return ((char *)&str[i - j]);
		i = (i - j) + 1;
	}
	return ((char *)0);
}
