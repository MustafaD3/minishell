/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 12:55:31 by makboga           #+#    #+#             */
/*   Updated: 2025/06/28 15:13:28 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_mutexes(char *str, t_sim *sim)
{
	int	i;

	if (str)
	{
		write(2, str, ft_strlen(str));
		write(2, "\n", 1);
	}
	pthread_mutex_destroy(&sim->write_lock);
	pthread_mutex_destroy(&sim->meal_lock);
	pthread_mutex_destroy(&sim->dead_lock);
	if (sim->forks)
	{
		i = 0;
		while (i < sim->num_of_philos)
			pthread_mutex_destroy(&sim->forks[i++]);
		free(sim->forks);
	}
	if (sim->philos)
	{
		i = 0;
		while (i < sim->num_of_philos)
			pthread_mutex_destroy(&sim->philos[i++].mutex);
		free(sim->philos);
	}
}

int	ft_strlen(char *str)
{
	int	i;

	if (str == NULL)
		return (EXIT_FAILURE);
	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

bool	ft_isspace(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (true);
	return (false);
}

bool	ft_is_digit(char c)
{
	if (c >= '0' && c <= '9')
		return (true);
	return (false);
}

int	ft_atoi(const char *str)
{
	int				i;
	long			sign;
	long long		result;

	i = 0;
	result = 0;
	sign = 1;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		if ((sign == 1 && result > INT_MAX)
			|| (sign == -1 && - result < INT_MIN))
			return (-1);
		i++;
	}
	return ((int)(result * sign));
}
