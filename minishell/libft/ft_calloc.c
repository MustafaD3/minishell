/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 15:35:49 by makboga           #+#    #+#             */
/*   Updated: 2025/08/16 15:35:51 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t num_blocks, size_t block_size)
{
	char	*str;

	str = (char *)malloc(num_blocks * block_size);
	if (!str)
		return (NULL);
	ft_bzero(str, num_blocks * block_size);
	return (str);
}
