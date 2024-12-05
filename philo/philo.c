/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoklova <msoklova@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 15:39:41 by msoklova          #+#    #+#             */
/*   Updated: 2024/12/05 13:39:56 by msoklova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

void	clean_mutex(t_events *events)
{
	int	i;

	i = 0;
	while (i < events->philo_num)
	{
		pthread_mutex_destroy(events->forks[i].lock_fork);
		free(events->forks[i].lock_fork);
		i++;
	}
	i = 0;
	while (i < events->philo_num)
	{
		pthread_mutex_destroy(&events->philo[i].philo_lock);
		i++;
	}
	pthread_mutex_destroy(&events->meal_lock);
	pthread_mutex_destroy(events->dead_mutex);
	pthread_mutex_destroy(&events->print_lock);
	free(events->dead_mutex);
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
	clean_mutex(events);
	free(events->philo);
	free(events->forks);
	free(events);
	return (0);
}
