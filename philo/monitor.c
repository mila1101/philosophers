/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoklova <msoklova@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:06:16 by msoklova          #+#    #+#             */
/*   Updated: 2024/12/04 14:41:01 by msoklova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_philosopher_death(t_events *events)
{
	int		i;
	long	since_last_meal;

	i = 0;
	while (i < events->philo_num)
	{
		pthread_mutex_lock(&events->philo[i].philo_lock);
		since_last_meal = curr_time() - events->philo[i].last_meal_time;
		if (since_last_meal > events->time_to_die)
		{
			print_action(events, events->philo[i].id, "died");
			pthread_mutex_lock(events->dead_mutex);
			events->dead = 1;
			pthread_mutex_unlock(events->dead_mutex);
			pthread_mutex_unlock(&events->philo[i].philo_lock);
			return (1);
		}
		pthread_mutex_unlock(&events->philo[i].philo_lock);
		i++;
		usleep(10);
	}
	return (0);
}

static int	check_all_ate(t_events *events)
{
	int	i;
	int	all_ate;

	i = 0;
	all_ate = 1;
	while (i < events->philo_num)
	{
		pthread_mutex_lock(&events->philo[i].philo_lock);
		if (events->meals_needed != -1
			&& events->philo[i].meals_eaten < events->meals_needed)
			all_ate = 0;
		pthread_mutex_unlock(&events->philo[i].philo_lock);
		i++;
		usleep(10);
	}
	if (all_ate && events->meals_needed != -1)
	{
		return (1);
	}
	return (0);
}

void	*death_monitor(void *arg)
{
	t_events	*events;

	events = (t_events *)arg;
	while (1)
	{
		pthread_mutex_lock(events->dead_mutex);
		if (events->dead)
		{
			pthread_mutex_unlock(events->dead_mutex);
			break ;
		}
		pthread_mutex_unlock(events->dead_mutex);
		if (check_philosopher_death(events))
			return (NULL);
		if (check_all_ate(events))
			break ;
		usleep(100);
	}
	return (NULL);
}

