/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 15:37:58 by makboga           #+#    #+#             */
/*   Updated: 2025/08/16 15:38:00 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int number, int fd)
{
	if (number < 0)
	{
		number *= -1;
		ft_putchar_fd('-', fd);
	}
	if (number == -2147483648)
	{
		ft_putchar_fd('2', fd);
		number = 147483648;
	}
	if (number < 10)
		ft_putchar_fd(number + '0', fd);
	else
	{
		ft_putnbr_fd(number / 10, fd);
		ft_putnbr_fd(number % 10, fd);
	}
}
