/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oonal <oonal@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 19:20:04 by oonal             #+#    #+#             */
/*   Updated: 2025/08/23 19:28:00 by oonal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_philosopher_death(t_all *all, int i)
{
	long	current_time;
	long	last_meal;

	pthread_mutex_lock(&all->philos[i].meal_mutex);
	current_time = timestamp();
	last_meal = all->philos[i].last_meal;
	pthread_mutex_unlock(&all->philos[i].meal_mutex);
	if (current_time - last_meal > all->time_to_die)
	{
		pthread_mutex_lock(&all->death_mutex);
		all->someone_died = 1;
		pthread_mutex_unlock(&all->death_mutex);
		pthread_mutex_lock(&all->print_mutex);
		printf("%ld %d died\n",
			current_time - all->start_time, all->philos[i].id);
		pthread_mutex_unlock(&all->print_mutex);
		return (1);
	}
	return (0);
}

static int	check_all_philosophers_ate(t_all *all)
{
	int	i;
	int	ate_count;

	if (all->must_eat_count < 0)
		return (0);
	i = 0;
	while (i < all->nb_philo)
	{
		pthread_mutex_lock(&all->philos[i].meal_mutex);
		ate_count = all->philos[i].eat_count;
		pthread_mutex_unlock(&all->philos[i].meal_mutex);
		if (ate_count < all->must_eat_count)
			return (0);
		i++;
	}
	pthread_mutex_lock(&all->death_mutex);
	all->all_ate_enough = 1;
	pthread_mutex_unlock(&all->death_mutex);
	return (1);
}

void	*monitor_routine(void *arg)
{
	t_all	*all;
	int		i;

	all = (t_all *)arg;
	while (!check_death(all) && !check_all_ate(all))
	{
		i = 0;
		while (i < all->nb_philo && !check_death(all))
		{
			if (check_philosopher_death(all, i))
				return (NULL);
			i++;
		}
		if (check_all_philosophers_ate(all))
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
