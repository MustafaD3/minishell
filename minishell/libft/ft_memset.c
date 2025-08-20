/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 15:37:45 by makboga           #+#    #+#             */
/*   Updated: 2025/08/16 15:37:47 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *str, int chr, size_t len)
{
	unsigned char	*s;

	s = (unsigned char *)str;
	while (len--)
		*(s++) = chr;
	return (str);
}
