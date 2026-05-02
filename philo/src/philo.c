#include "philo.h"

static void print_action(t_philo *philo, const char *message)
{
	t_all	*all;
	long	ts;

	all = philo->all;
	pthread_mutex_lock(&all->death_mutex);
	if (all->someone_died)
	{
		pthread_mutex_unlock(&all->death_mutex);
		return;
	}
	pthread_mutex_lock(&all->print_mutex);
	ts = timestamp() - all->start_time;
	printf("%ld %d %s\n", ts, philo->id, message);
	pthread_mutex_unlock(&all->print_mutex);
	pthread_mutex_unlock(&all->death_mutex);
}

static int	take_forks(t_all *all, t_philo *philo)
{
	(void)all;
	if (check_death(philo->all))
		return (1);
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		print_action(philo, "has taken a fork");
		if ((philo->all->nb_philo % 2) == 1 && (philo->id % 2) == 1)
			usleep(200);
		if (check_death(philo->all))
		{
			pthread_mutex_unlock(philo->left_fork);
			return (1);
		}
		pthread_mutex_lock(philo->right_fork);
		print_action(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		print_action(philo, "has taken a fork");
		if ((philo->all->nb_philo % 2) == 1 && (philo->id % 2) == 1)
			usleep(200);
		if (check_death(philo->all))
		{
			pthread_mutex_unlock(philo->right_fork);
			return 1;
		}
		pthread_mutex_lock(philo->left_fork);
		print_action(philo, "has taken a fork");
	}
	return (0);
}

static int eat(t_all *all, t_philo *philo)
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

static int sleep_and_think(t_all *all, t_philo *philo)
{
	long sleep;

	all = philo->all;
	if (check_death(all))
		return 1;
	print_action(philo, "is sleeping");
	precise_sleep(all->time_to_sleep);
	if (check_death(all))
		return 1;
	print_action(philo, "is thinking");
	if (all->nb_philo > 1)
	{
		if ((all->nb_philo % 2) == 0)
			sleep = 1;
		else
			sleep = 0;
		if (sleep > 0)
			precise_sleep(sleep % 2);
	}
	return 0;
}

void *philosopher_routine(void *arg)
{
	t_philo	*philo;
	t_all	*all;


	philo = (t_philo *)arg;
	all = philo->all;
	if ((all->nb_philo % 2) == 0 && (philo->id % 2 == 0))
		usleep(all->time_to_eat * 500);
	while (!check_death(all) && !check_all_ate(all))
	{
		if (take_forks(all, philo))
			break;
		if (eat(all, philo))
		{
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_unlock(philo->right_fork);
			break;
		}
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		if (sleep_and_think(all, philo))
			break;
	}
	return (NULL);
}

void *single_philosopher_routine(void *arg)
{
	t_philo *philo = (t_philo *)arg;
	t_all   *all = philo->all;
	long    ts;

	philo = (t_philo *)arg;
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
	return NULL;
}
