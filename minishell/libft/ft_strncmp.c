/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 15:38:53 by makboga           #+#    #+#             */
/*   Updated: 2025/08/16 15:38:55 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *str1, const char *str2, size_t size)
{
	int	idx;

	idx = 0;
	while ((size != 0) && (str1[idx] != '\0') && (str1[idx] == str2[idx]))
	{
		idx++;
		size--;
	}
	if (size != 0)
		return ((unsigned char)str1[idx] - (unsigned char)str2[idx]);
	else
		return (0);
}
