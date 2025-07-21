/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkilic344@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 08:32:29 by mdalkili          #+#    #+#             */
/*   Updated: 2025/07/18 16:50:23 by mdalkili         ###   ########.fr       */
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