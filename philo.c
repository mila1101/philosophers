/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoklova <msoklova@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 15:39:41 by msoklova          #+#    #+#             */
/*   Updated: 2024/12/04 14:48:09 by msoklova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	if_ended(t_events *events)
{
	int	ended;

	pthread_mutex_lock(events->dead_mutex);
	ended = events->dead;
	pthread_mutex_unlock(events->dead_mutex);
	return (ended);
}

static int	meal_death_check(t_philo *philo, t_events *events)
{
	pthread_mutex_lock(events->dead_mutex);
	if (events->dead || (events->meals_needed != -1 && philo->meals_eaten >= events->meals_needed))
	{
		pthread_mutex_unlock(events->dead_mutex);
		return (1);
	}
	pthread_mutex_unlock(events->dead_mutex);
	return (0);
}

void	*routine(void *arg)
{
	t_philo		*philo;
	t_events	*events;
	int			l_fork;
	int			r_fork;

	philo = (t_philo *)arg;
	i_philo(philo, &events, &l_fork, &r_fork);
	if (events->starter != 1)
		;
	while (!events->dead)
	{
		if (meal_death_check(philo, events))
			break;
		if (take_forks(events, l_fork, r_fork, philo))
			break ;
		eat(philo, events, l_fork, r_fork);
		release_forks(events, l_fork, r_fork);
		if (if_ended(events))
			break ;
		sleep_and_think(philo, events);
	}
	return (NULL);
}

int	check_args(int argc)
{
	if (argc < 5 || argc > 6)
	{
		printf("input: [philos] [death] [eat] [sleep] optional: [meals]\n");
		return (1);
	}
	return (0);
}

int	init_threads(t_events *events, pthread_t *monitor)
{
	int	i;

	i = 0;
	while (i < events->philo_num)
	{
		if (pthread_create(&events->philo[i].thread,
				NULL, routine, &events->philo[i]))
			return (0);
		usleep(10);
		i++;
	}
	events->starter = 1;
	if (pthread_create(monitor, NULL, death_monitor, events))
		return (0);
	return (1);
}

void	join_threads(t_events *events, pthread_t monitor)
{
	int	i;

	i = 0;
	while (i < events->philo_num)
	{
		pthread_join(events->philo[i].thread, NULL);
		i++;
	}
	pthread_join(monitor, NULL);
}

int	main(int argc, char **argv)
{
	t_events	*events;
	pthread_t	monitor;

	if (check_args(argc))
		return (1);
	events = init_events(argv);
	if (!events)
		return (1);
	if (!init_threads(events, &monitor))
		return (1);
	join_threads(events, monitor);
	pthread_mutex_destroy(&events->meal_lock);
	pthread_mutex_destroy(events->dead_mutex);
	return (0);
}
