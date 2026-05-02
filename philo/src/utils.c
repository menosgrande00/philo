/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oonal <oonal@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 19:19:45 by oonal             #+#    #+#             */
/*   Updated: 2025/08/23 19:27:34 by oonal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int					i;
	int					sign;
	unsigned long int	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result *= 10;
		result += str[i] - '0';
		i++;
	}
	return ((int)(result * sign));
}

long	timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	precise_sleep(long ms)
{
	long	start;

	start = timestamp();
	while (timestamp() - start < ms)
		usleep(300);
}

int	check_death(t_all *all)
{
	pthread_mutex_lock(&all->death_mutex);
	if (all->someone_died)
	{
		pthread_mutex_unlock(&all->death_mutex);
		return (1);
	}
	pthread_mutex_unlock(&all->death_mutex);
	return (0);
}

int	check_all_ate(t_all *all)
{
	pthread_mutex_lock(&all->death_mutex);
	if (all->all_ate_enough)
	{
		pthread_mutex_unlock(&all->death_mutex);
		return (1);
	}
	pthread_mutex_unlock(&all->death_mutex);
	return (0);
}
