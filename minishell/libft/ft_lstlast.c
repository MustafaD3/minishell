/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 15:37:06 by makboga           #+#    #+#             */
/*   Updated: 2025/08/16 15:37:07 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	t_list	*current_unit;

	if (!lst)
		return (0);
	current_unit = lst;
	while (current_unit->next)
		current_unit = current_unit->next;
	return (current_unit);
}
