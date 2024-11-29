/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoklova <msoklova@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 15:39:41 by msoklova          #+#    #+#             */
/*   Updated: 2024/11/29 16:17:09 by msoklova         ###   ########.fr       */
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

void *routine(void *arg)
{
	t_philo		*philo;
	t_events	*events;
	int			l_fork;
	int			r_fork;

	philo = (t_philo *)arg;
	events = philo->events;
	l_fork = philo->id - 1;
	r_fork = philo->id % events->philo_num;
	pthread_mutex_lock(&philo->philo_lock);
	philo->last_meal_time = curr_time();
	pthread_mutex_unlock(&philo->philo_lock);
	print_action(events, philo->id, "is thinking");
	if (philo->id % 2 == 0)
		ft_usleep(events->time_to_eat / 2);
	while (!events->dead)
	{
		pthread_mutex_lock(events->dead_mutex);
		if (events->dead || (events->meals_needed != -1 && philo->meals_eaten >= events->meals_needed))
		{
			pthread_mutex_unlock(events->dead_mutex);
			break ;
		}
		pthread_mutex_unlock(events->dead_mutex);
		if (l_fork < r_fork)
		{
			pthread_mutex_lock(events->forks[l_fork].lock_fork);
			if (if_ended(events))
			{
				pthread_mutex_unlock(events->forks[l_fork].lock_fork);
				break ;
			}
			print_action(events, philo->id, "has taken a fork");
			if (events->philo_num == 1)
			{
				ft_usleep(events->time_to_die);
				return (pthread_mutex_unlock(events->forks[l_fork].lock_fork), NULL);
			}
			pthread_mutex_lock(events->forks[r_fork].lock_fork);
			if (if_ended(events))
			{
				pthread_mutex_unlock(events->forks[l_fork].lock_fork);
				pthread_mutex_unlock(events->forks[r_fork].lock_fork);
				break ;
			}
			print_action(events, philo->id, "has taken a fork");
		}
		else
		{
			pthread_mutex_lock(events->forks[r_fork].lock_fork);
			if (if_ended(events))
			{
				pthread_mutex_unlock(events->forks[r_fork].lock_fork);
				break ;
			}
			print_action(events, philo->id, "has taken a fork");
			if (events->philo_num == 1)
			{
				ft_usleep(events->time_to_die);
				return (pthread_mutex_unlock(events->forks[r_fork].lock_fork), NULL);
			}
			pthread_mutex_lock(events->forks[l_fork].lock_fork);
			if (if_ended(events))
			{
				pthread_mutex_unlock(events->forks[r_fork].lock_fork);
				pthread_mutex_unlock(events->forks[l_fork].lock_fork);
				break ;
			}
			print_action(events, philo->id, "has taken a fork");
		}
		pthread_mutex_lock(&philo->philo_lock);
		philo->last_meal_time = curr_time();
		print_action(events, philo->id, "is eating");
		philo->meals_eaten++;
		pthread_mutex_unlock(&philo->philo_lock);
		if (philo->meals_eaten == events->meals_needed)
		{
			pthread_mutex_lock(&events->meal_lock);
			events->eaten++;
			pthread_mutex_unlock(&events->meal_lock);
		}
		ft_usleep(events->time_to_eat);
		pthread_mutex_unlock(events->forks[r_fork].lock_fork);
		pthread_mutex_unlock(events->forks[l_fork].lock_fork);
		if (if_ended(events))
			break ;
		print_action(events, philo->id, "is sleeping");
		ft_usleep(events->time_to_sleep);
		if (if_ended(events))
			break ;
		print_action(events, philo->id, "is thinking");
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
		printf("Correct use: [philosophers] [death] [eat] [sleep] optional: [meals]\n");
		return (1);
	}
	events = init_events(argv);
	if (!events)
		return (1);
	i = 0;
	while (i < events->philo_num)
	{
		pthread_create(&events->philo[i].thread, NULL, routine, &events->philo[i]);
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
