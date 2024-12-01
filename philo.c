/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoklova <msoklova@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 15:39:41 by msoklova          #+#    #+#             */
/*   Updated: 2024/12/01 14:15:08 by msoklova         ###   ########.fr       */
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

void	*routine(void *arg)
{
	t_philo		*philo;
	t_events	*events;
	int			l_fork;
	int			r_fork;

	philo = (t_philo *)arg;
	i_philo(philo, &events, &l_fork, &r_fork);
	while (!events->dead)
	{
		pthread_mutex_lock(events->dead_mutex);
		if (events->dead || (events->meals_needed != -1 && philo->meals_eaten >= events->meals_needed))
		{
			pthread_mutex_unlock(events->dead_mutex);
			break ;
		}
		pthread_mutex_unlock(events->dead_mutex);
		if (take_forks(events, l_fork, r_fork, philo))
			break ;
		eat(philo, events);
		release_forks(events, l_fork, r_fork);
		if (if_ended(events))
			break ;
		sleep_and_think(philo, events);
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_events	*events;
	pthread_t	monitor;
	int			i;

	if (argc < 5 || argc > 6)
	{
		printf("input: [philos] [death] [eat] [sleep] optional: [meals]\n");
		return (1);
	}
	events = init_events(argv);
	if (!events)
		return (1);
	i = 0;
	while (i < events->philo_num)
	{
		pthread_create(&events->philo[i].thread,
			NULL, routine, &events->philo[i]);
		i++;
	}
	pthread_create(&monitor, NULL, death_monitor, events);
	i = 0;
	while (i < events->philo_num)
	{
		pthread_join(events->philo[i].thread, NULL);
		i++;
	}
	pthread_join(monitor, NULL);
	pthread_mutex_destroy(&events->meal_lock);
	pthread_mutex_destroy(events->dead_mutex);
	return (0);
}
