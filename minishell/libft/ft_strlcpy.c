/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 15:38:35 by makboga           #+#    #+#             */
/*   Updated: 2025/08/16 15:38:36 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	idx;
	size_t	lenght;

	lenght = ft_strlen(src);
	if (dstsize)
	{
		idx = 0;
		while ((*src) && (idx < (dstsize - 1)))
			dst[idx++] = *src++;
		dst[idx] = '\0';
	}
	return (lenght);
}
