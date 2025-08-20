/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 15:36:45 by makboga           #+#    #+#             */
/*   Updated: 2025/08/16 15:36:47 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*current_unit;

	if (!lst || !(*lst))
	{
		*lst = new;
		return ;
	}
	current_unit = *lst;
	while (current_unit->next != NULL)
		current_unit = current_unit->next;
	current_unit->next = new;
}
