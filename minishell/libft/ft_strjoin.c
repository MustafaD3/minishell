/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 15:38:23 by makboga           #+#    #+#             */
/*   Updated: 2025/08/16 15:38:24 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*assemble;
	size_t	index;

	if ((!s1) || (!s2))
		return (ft_strdup(""));
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	assemble = (char *)malloc((ft_strlen(s1) + ft_strlen(s2)) + 1);
	if (!assemble)
		return (NULL);
	index = -1;
	while (++index < (ft_strlen(s1) + ft_strlen(s2)))
	{
		if (index < ft_strlen(s1))
			assemble[index] = s1[index];
		else
			assemble[index] = s2[index - ft_strlen(s1)];
	}
	assemble[index] = '\0';
	return (assemble);
}
