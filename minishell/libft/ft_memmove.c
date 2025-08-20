/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 15:37:41 by makboga           #+#    #+#             */
/*   Updated: 2025/08/16 15:37:43 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *source, size_t n)
{
	char	*dst;
	char	*src;

	dst = (char *)dest;
	src = (char *)source;
	if (!dest && !source)
		return (0);
	if (dest < source)
		return (ft_memcpy(dest, source, n));
	while (n--)
		dst[n] = src[n];
	return (dest);
}
