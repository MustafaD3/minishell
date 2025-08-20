/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 15:37:22 by makboga           #+#    #+#             */
/*   Updated: 2025/08/16 15:37:23 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	t_list	*current_unit;
	int		counter;

	if (!lst)
		return (0);
	counter = 0;
	current_unit = lst;
	while (current_unit)
	{
		current_unit = current_unit->next;
		counter++;
	}
	return (counter);
}
