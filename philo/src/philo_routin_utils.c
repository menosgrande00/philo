/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routin_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oonal <oonal@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 19:19:58 by oonal             #+#    #+#             */
/*   Updated: 2025/08/23 19:24:45 by oonal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	take_even_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_action(philo, "has taken a fork");
	if (check_death(philo->all))
	{
		pthread_mutex_unlock(philo->left_fork);
		return (1);
	}
	pthread_mutex_lock(philo->right_fork);
	print_action(philo, "has taken a fork");
	return (0);
}

static int	take_odd_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->right_fork);
	print_action(philo, "has taken a fork");
	if (check_death(philo->all))
	{
		pthread_mutex_unlock(philo->right_fork);
		return (1);
	}
	pthread_mutex_lock(philo->left_fork);
	print_action(philo, "has taken a fork");
	return (0);
}

int	take_forks(t_philo *philo)
{
	if (check_death(philo->all))
		return (1);
	if (philo->id % 2 == 0)
		return (take_even_forks(philo));
	else
		return (take_odd_forks(philo));
}

int	eat(t_all *all, t_philo *philo)
{
	all = philo->all;
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = timestamp();
	philo->eat_count++;
	pthread_mutex_unlock(&philo->meal_mutex);
	print_action(philo, "is eating");
	if (check_death(all))
		return (1);
	precise_sleep(all->time_to_eat);
	return (0);
}

int	sleep_and_think(t_all *all, t_philo *philo)
{
	long	delay;
	int		deadline;

	all = philo->all;
	if (check_death(all))
		return (1);
	print_action(philo, "is sleeping");
	precise_sleep(all->time_to_sleep);
	if (check_death(all))
		return (1);
	print_action(philo, "is thinking");
	deadline = (all->time_to_die
			<= (all->time_to_eat * 2 + all->time_to_sleep));
	if (!deadline && all->nb_philo > 1)
	{
		if (all->nb_philo % 2 == 1)
			delay = all->time_to_eat / 6;
		else
			delay = 1;
		if (delay > 0)
			precise_sleep(delay);
	}
	return (0);
}
