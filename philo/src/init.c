/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oonal <oonal@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 19:20:11 by oonal             #+#    #+#             */
/*   Updated: 2025/08/23 19:28:54 by oonal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_philo(t_all *all)
{
	int	i;

	all->philos = malloc(sizeof(t_philo) * all->nb_philo);
	if (!all->philos)
		return (1);
	i = -1;
	while (++i < all->nb_philo)
	{
		all->philos[i].id = i + 1;
		all->philos[i].eat_count = 0;
		all->philos[i].last_meal = all->start_time;
		all->philos[i].all = all;
		all->philos[i].left_fork = &all->forks[i];
		all->philos[i].right_fork = &all->forks[(i + 1) % all->nb_philo];
		if (pthread_mutex_init(&all->philos[i].meal_mutex, NULL))
		{
			while (--i >= 0)
				pthread_mutex_destroy(&all->philos[i].meal_mutex);
			free(all->philos);
			return (1);
		}
	}
	return (0);
}

static int	init_other_mutexes(t_all *all)
{
	int	i;

	if (pthread_mutex_init(&all->print_mutex, NULL))
	{
		i = all->nb_philo;
		while (--i >= 0)
			pthread_mutex_destroy(&all->forks[i]);
		free(all->forks);
		return (1);
	}
	if (pthread_mutex_init(&all->death_mutex, NULL))
	{
		pthread_mutex_destroy(&all->print_mutex);
		i = all->nb_philo;
		while (--i >= 0)
			pthread_mutex_destroy(&all->forks[i]);
		free(all->forks);
		return (1);
	}
	return (0);
}

int	init_mutex(t_all *all)
{
	int	i;

	all->forks = malloc(sizeof(pthread_mutex_t) * all->nb_philo);
	if (!all->forks)
		return (1);
	i = -1;
	while (++i < all->nb_philo)
	{
		if (pthread_mutex_init(&all->forks[i], NULL))
		{
			while (--i >= 0)
				pthread_mutex_destroy(&all->forks[i]);
			free(all->forks);
			return (1);
		}
	}
	i = init_other_mutexes(all);
	return (i);
}

int	init_all(t_all *all, int argc, char **argv)
{
	if (argc != 5 && argc != 6)
		return (1);
	all->nb_philo = ft_atoi(argv[1]);
	all->time_to_die = ft_atoi(argv[2]);
	all->time_to_eat = ft_atoi(argv[3]);
	all->time_to_sleep = ft_atoi(argv[4]);
	if (all->nb_philo <= 0 || all->nb_philo > 200 || all->time_to_die < 60
		|| all->time_to_eat < 60 || all->time_to_sleep < 60)
		return (1);
	if (argc == 6)
	{
		all->must_eat_count = ft_atoi(argv[5]);
		if (all->must_eat_count <= 0)
			return (1);
	}
	else
		all->must_eat_count = -1;
	all->someone_died = 0;
	all->all_ate_enough = 0;
	all->start_time = timestamp();
	if (init_mutex(all))
		return (2);
	if (init_philo(all))
		return (3);
	return (0);
}

void	cleanup_all(t_all *all)
{
	int	i;

	if (all->philos)
	{
		i = 0;
		while (i < all->nb_philo)
		{
			pthread_mutex_destroy(&all->philos[i].meal_mutex);
			i++;
		}
		free(all->philos);
	}
	if (all->forks)
	{
		i = 0;
		while (i < all->nb_philo)
		{
			pthread_mutex_destroy(&all->forks[i]);
			i++;
		}
		free(all->forks);
	}
	pthread_mutex_destroy(&all->print_mutex);
	pthread_mutex_destroy(&all->death_mutex);
}
