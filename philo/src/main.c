/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oonal <oonal@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 19:20:08 by oonal             #+#    #+#             */
/*   Updated: 2025/08/23 19:28:16 by oonal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_action(t_philo *philo, const char *message)
{
	t_all	*all;
	long	ts;

	all = philo->all;
	pthread_mutex_lock(&all->death_mutex);
	if (all->someone_died)
	{
		pthread_mutex_unlock(&all->death_mutex);
		return ;
	}
	pthread_mutex_lock(&all->print_mutex);
	ts = timestamp() - all->start_time;
	printf("%ld %d %s\n", ts, philo->id, message);
	pthread_mutex_unlock(&all->print_mutex);
	pthread_mutex_unlock(&all->death_mutex);
}

void	*single_ph(void *arg)
{
	t_philo	*philo;
	t_all	*all;
	long	ts;

	philo = (t_philo *)arg;
	all = philo->all;
	pthread_mutex_lock(philo->left_fork);
	print_action(philo, "has taken a fork");
	precise_sleep(all->time_to_die);
	pthread_mutex_lock(&all->death_mutex);
	all->someone_died = 1;
	pthread_mutex_unlock(&all->death_mutex);
	pthread_mutex_lock(&all->print_mutex);
	ts = timestamp() - all->start_time;
	printf("%ld %d died\n", ts, philo->id);
	pthread_mutex_unlock(&all->print_mutex);
	pthread_mutex_unlock(philo->left_fork);
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_all		all;
	pthread_t	monitor;
	int			i;

	i = init_all(&all, argc, argv);
	if (i > 0)
		return (1);
	if (all.nb_philo == 1)
	{
		pthread_create(&all.philos[0].thread, NULL, single_ph, &all.philos[0]);
		pthread_join(all.philos[0].thread, NULL);
	}
	else
	{
		i = -1;
		while (++i < all.nb_philo)
			pthread_create(&all.philos[i].thread, NULL, ph_r, &all.philos[i]);
		pthread_create(&monitor, NULL, monitor_routine, &all);
		i = -1;
		while (++i < all.nb_philo)
			pthread_join(all.philos[i].thread, NULL);
		pthread_join(monitor, NULL);
	}
	cleanup_all(&all);
	return (0);
}
