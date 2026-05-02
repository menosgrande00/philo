/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oonal <oonal@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 19:19:49 by oonal             #+#    #+#             */
/*   Updated: 2025/08/23 19:30:52 by oonal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

struct	s_all;

typedef struct s_philo {
	int				id;
	int				eat_count;
	long			last_meal;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	meal_mutex;
	struct s_all	*all;
}	t_philo;

typedef struct s_all {
	int				nb_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat_count;
	int				someone_died;
	long			start_time;
	int				all_ate_enough;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	death_mutex;
}	t_all;

long	timestamp(void);
void	precise_sleep(long time);
int		ft_atoi(const char *str);
int		check_death(t_all *all);
int		check_all_ate(t_all *all);

int		init_all(t_all *all, int argc, char **argv);
int		init_philo(t_all *all);
int		init_mutex(t_all *all);
void	*ph_r(void *arg);
void	*single_ph(void *arg);
void	*monitor_routine(void *arg);
void	cleanup_all(t_all *all);
void	print_action(t_philo *philo, const char *message);

int		sleep_and_think(t_all *all, t_philo *philo);
int		eat(t_all *all, t_philo *philo);
int		take_forks(t_philo *philo);

#endif