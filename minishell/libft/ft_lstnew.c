/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 15:37:18 by makboga           #+#    #+#             */
/*   Updated: 2025/08/16 15:37:19 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*unit;

	unit = (t_list *)malloc(sizeof(t_list));
	if (!unit)
		return (NULL);
	unit->content = content;
	unit->next = NULL;
	return (unit);
}
