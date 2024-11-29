/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoklova <msoklova@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:06:16 by msoklova          #+#    #+#             */
/*   Updated: 2024/11/29 13:23:36 by msoklova         ###   ########.fr       */
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
	while (1)
	{
		pthread_mutex_lock(events->dead_mutex);
		if (events->dead)
		{
			pthread_mutex_unlock(events->dead_mutex);
			break ;
		}
		pthread_mutex_unlock(events->dead_mutex);
		all_ate = 1;
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
				return (NULL);
			}
			//pthread_mutex_lock(&events->philo[i].philo_lock);
			if (events->meals_needed != -1 && events->philo[i].meals_eaten < events->meals_needed)
				all_ate = 0;
			pthread_mutex_unlock(&events->philo[i].philo_lock);
			i++;
		}
		if (all_ate && events->meals_needed != -1)
		{
			pthread_mutex_lock(events->dead_mutex);
			events->dead = 1;
			pthread_mutex_unlock(events->dead_mutex);
			break ;
		}
		usleep(100);
	}
	return (NULL);
}

//void *death_monitor(void *arg)
//{
//	t_events	*events;
//	int			i;
//	long		since_last_meal;
//	int			all_ate;

//	events  = (t_events *)arg;
//	while (1)
//	{
//		pthread_mutex_lock(events->dead_mutex);
//		if (events->dead)
//		{
//			pthread_mutex_unlock(events->dead_mutex);
//			break ;
//		}
//		pthread_mutex_unlock(events->dead_mutex);
//		all_ate = 1;
//		i = 0;
//		while (i < events->philo_num)
//		{
//			pthread_mutex_lock(&events->philo[i].philo_lock);
//			since_last_meal = curr_time() - events->philo[i].last_meal_time;
//			pthread_mutex_unlock(&events->philo[i].philo_lock);
//			if (since_last_meal > events->time_to_die)
//			{
//				pthread_mutex_lock(events->dead_mutex);
//				events->dead = 1;
//				pthread_mutex_unlock(events->dead_mutex);
//				print_action(events, events->philo[i].id, "died");
//				pthread_mutex_unlock(&events->philo[i].philo_lock);
//				return (pthread_mutex_unlock(&events->philo[i].philo_lock), NULL) ;
//			}
//			pthread_mutex_lock(&events->philo[i].philo_lock);
//			if (events->meals_needed != -1 && events->philo[i].meals_eaten < events->meals_needed)
//				all_ate = 0;
//			pthread_mutex_unlock(&events->philo[i].philo_lock);
//			i++;
//		}
//		if (all_ate && events->meals_needed != -1)
//		{
//			pthread_mutex_lock(events->dead_mutex);
//			events->dead = 1;
//			pthread_mutex_unlock(events->dead_mutex);
//			break ;
//		}
//		usleep(100);
//	}
//	return (NULL);
//}
