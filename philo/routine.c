/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoklova <msoklova@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 10:37:38 by msoklova          #+#    #+#             */
/*   Updated: 2024/12/05 10:41:49 by msoklova         ###   ########.fr       */
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
	if (events->dead || (events->meals_needed != -1
			&& philo->meals_eaten >= events->meals_needed))
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
	{
	}
	while (!events->dead)
	{
		if (meal_death_check(philo, events))
			break ;
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
