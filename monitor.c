/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoklova <msoklova@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:06:16 by msoklova          #+#    #+#             */
/*   Updated: 2024/11/05 16:18:26 by msoklova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *death_monitor(void *arg)
{
	t_events	*events;
	int			i;
	long		since_last_meal;

	events  = (t_events *)arg;
	while (!events->dead)
	{
		i = 0;
		while(i < events->philo_num)
		{
			pthread_mutex_lock(&events->philo[i].philo_lock);
			since_last_meal = curr_time() - events->philo[i].last_meal_time;
			if(since_last_meal > events->time_to_die)
			{
				events->dead = 1;
				print_action(events, events->philo[i].id, "died");
				pthread_mutex_unlock(&events->philo[i].philo_lock);
				exit (0);
			}
			pthread_mutex_unlock(&events->philo[i].philo_lock);
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}