/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 12:17:26 by makboga           #+#    #+#             */
/*   Updated: 2025/07/03 13:36:18 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

# define PHILO_MAX 200

# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define MAGENTA "\033[35m"
# define CYAN "\033[36m"
# define WHITE "\033[37m"
# define RESET "\033[0m"

typedef struct s_sim	t_sim;

typedef struct s_philo
{
	int					id;
	int					meals_eaten;
	int					eating;
	size_t				last_meal;
	pthread_t			thread;
	pthread_mutex_t		*l_fork;
	pthread_mutex_t		*r_fork;
	t_sim				*sim;
	pthread_mutex_t		mutex;
}						t_philo;

typedef struct s_sim
{
	int					num_of_philos;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					number_of_must_eat;
	int					full_philos;
	int					someone_died;
	size_t				start_time;
	pthread_mutex_t		write_lock;
	pthread_mutex_t		dead_lock;
	pthread_mutex_t		meal_lock;
	pthread_mutex_t		full_lock;
	pthread_mutex_t		*forks;
	t_philo				*philos;
	struct timeval		begin_time;
}						t_sim;

void					ft_error(const char *str);
void					destroy_mutexes(char *str, t_sim *sim);
bool					ft_isspace(char c);
bool					ft_is_digit(char c);
int						ft_atoi(const char *str);
void					init_forks(t_sim *sim);
void					init_philos(t_sim *sim);
void					start_threads(t_sim *sim);
void					*philo_routine(void *arg);
long					get_current_time(void);
void					print_status(t_philo *philo, char *message,
							char *color);
void					*ft_monitor(void *arg);
int						philo_has_died(t_philo *philo);
void					release_forks(t_philo *philo);
void					ft_usleep(long time_in_ms, t_philo *philo);
int						ft_strlen(char *str);
void					taken_forks(t_philo *philo);

#endif