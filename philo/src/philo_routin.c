/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oonal <oonal@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 19:19:38 by oonal             #+#    #+#             */
/*   Updated: 2025/08/23 19:19:39 by oonal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	philo_action(t_all *all, t_philo *philo)
{
	if (take_forks(philo))
		return (1);
	if (eat(all, philo))
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		return (1);
	}
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	if (all->must_eat_count >= 0)
	{
		pthread_mutex_lock(&philo->meal_mutex);
		if (philo->eat_count >= all->must_eat_count)
		{
			pthread_mutex_unlock(&philo->meal_mutex);
			return (1);
		}
		pthread_mutex_unlock(&philo->meal_mutex);
	}
	if (sleep_and_think(all, philo))
		return (1);
	return (0);
}

void	*ph_r(void *arg)
{
	t_philo	*philo;
	t_all	*all;
	int		tight_deadline;

	philo = (t_philo *)arg;
	all = philo->all;
	tight_deadline = (all->time_to_die <= (all->time_to_eat * 2
				+ all->time_to_sleep));
	if (!tight_deadline && (philo->id % 2 == 0))
		usleep(all->time_to_eat * 500);
	while (!check_death(all))
	{
		if (philo_action(all, philo))
			break ;
	}
	return (NULL);
}
