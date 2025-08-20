/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 16:20:19 by makboga           #+#    #+#             */
/*   Updated: 2025/08/16 16:25:23 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*string_concatation(char **str)
{
	char	*result;
	int		total_byte;
	int		i;

	if (!str)
		return (NULL);
	i = 0;
	total_byte = 0;
	while (str[i])
		total_byte += ft_strlen(str[i++]);
	result = malloc(total_byte + 1);
	if (!result)
		return (NULL);
	i = 0;
	total_byte = 0;
	while (str[i])
	{
		ft_memcpy(result + total_byte, str[i], ft_strlen(str[i]));
		total_byte += ft_strlen(str[i++]);
	}
	result[total_byte] = '\0';
	return (result);
}
