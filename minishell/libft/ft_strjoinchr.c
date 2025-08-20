/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoinchr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 15:38:26 by makboga           #+#    #+#             */
/*   Updated: 2025/08/16 15:38:26 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoinchr(char *str, char chr)
{
	char	*new_str;
	int		len;
	int		i;

	len = ft_strlen(str);
	new_str = (char *)malloc(sizeof(char) * (len + 2));
	i = -1;
	while (str[++i])
		new_str[i] = str[i];
	new_str[i] = chr;
	new_str[i + 1] = '\0';
	return (free(str), new_str);
}
