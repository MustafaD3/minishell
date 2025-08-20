/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 15:38:05 by makboga           #+#    #+#             */
/*   Updated: 2025/08/16 15:38:07 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	ft_len_of_word(char const *str, char c)
{
	int	counter;

	counter = 0;
	while (*str)
	{
		while (*str == c)
			str++;
		if (*str)
			counter++;
		while (*str != c && *str)
			str++;
	}
	return (counter);
}

static	void	ft_free(char **str)
{
	size_t	idx;

	idx = 0;
	while (str[idx])
	{
		free(str[idx]);
		idx++;
	}
	free(str);
}

static	char	**ft_return(char **arr, int idx)
{
	arr[idx] = 0;
	return (arr);
}

static	char	**ft_split_string(char **arr, char const *str, char c)
{
	int	idx;
	int	arr_idx;
	int	start;

	idx = 0;
	arr_idx = 0;
	while (str[idx])
	{
		while (str[idx] == c)
			idx++;
		start = idx;
		while (str[idx] != c && str[idx])
			idx++;
		if (idx > start)
		{
			arr[arr_idx] = ft_substr(str, start, idx - start);
			if (!arr[arr_idx])
			{
				ft_free(arr);
				return (NULL);
			}
			arr_idx++;
		}
	}
	return (ft_return(arr, arr_idx));
}

char	**ft_split(char const *str, char c)
{
	char	**arr;

	if (!str)
		return (NULL);
	arr = (char **)malloc((ft_len_of_word(str, c) + 1) * sizeof(char *));
	if (!arr)
		return (NULL);
	arr = ft_split_string(arr, str, c);
	return (arr);
}
