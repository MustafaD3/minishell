/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 15:39:10 by makboga           #+#    #+#             */
/*   Updated: 2025/08/16 16:08:35 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	begin;
	size_t	last;

	if (!s1 || !set)
		return (NULL);
	if (!*s1)
		return (ft_strdup(""));
	begin = 0;
	while ((s1[begin] != '\0') && (ft_strchr(set, s1[begin])))
		begin++;
	last = ft_strlen(s1);
	while ((begin < last) && (ft_strchr(set, s1[last - 1])))
		last--;
	return (ft_substr(s1, begin, last - begin));
}
