/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_two.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkilic344@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:57:18 by makboga           #+#    #+#             */
/*   Updated: 2025/08/19 00:36:27 by mdalkili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	free_array(char **matrix)
{
	int	i;

	i = 0;
	while (matrix && matrix[i])
		free(matrix[i++]);
	free(matrix);
}

char	*ft_strjoin_free(char *s1, const char *s2)
{
	char	*result;
	size_t	len1;
	size_t	len2;

	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	result = malloc(len1 + len2 + 1);
	if (!result)
	{
		free(s1);
		return (NULL);
	}
	ft_strlcpy(result, s1, len1 + 1);
	ft_strlcat(result, s2, len1 + len2 + 1);
	free(s1);
	return (result);
}

char	*set_and_free(char *dest, char *src)
{
	if (dest)
		free(dest);
	if (!src)
		return (NULL);
	return (src);
}
