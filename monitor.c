/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoklova <msoklova@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:06:16 by msoklova          #+#    #+#             */
/*   Updated: 2024/11/20 17:40:55 by msoklova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *death_monitor(void *arg)
{
	t_events	*events;
	int			i;
	long		since_last_meal;
	int			all_ate;

	events  = (t_events *)arg;
	while (!events->dead)
	{
		i = 0;
		while(i < events->philo_num)
		{
			pthread_mutex_lock(&events->philo[i].philo_lock);
			since_last_meal = curr_time() - events->philo[i].last_meal_time;
			if(since_last_meal >= events->time_to_die)
			{
				events->dead = 1;
				print_action(events, events->philo[i].id, "died");
				pthread_mutex_unlock(&events->philo[i].philo_lock);
				return (NULL);
			}
			pthread_mutex_unlock(&events->philo[i].philo_lock);
			i++;
		}
		if (events->meals_needed != -1)
		{
			all_ate = 1;
			i = 0;
			while (i < events->philo_num)
			{
				pthread_mutex_lock(&events->philo[i].philo_lock);
				if (events->philo[i].meals_eaten < events->meals_needed)
					all_ate = 0;
				pthread_mutex_unlock(&events->philo[i].philo_lock);
				i++;
			}
			if (events->meals_needed != -1 && all_ate)
				return (NULL);
		}
		//ft_usleep(1000);
		usleep(100);
	}
	return (NULL);
}