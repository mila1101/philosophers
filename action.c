/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoklova <msoklova@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 13:55:46 by msoklova          #+#    #+#             */
/*   Updated: 2024/12/01 14:15:03 by msoklova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	i_philo(t_philo *philo, t_events **events, int *l_fork, int *r_fork)
{
	*events = philo->events;
	*l_fork = philo->id - 1;
	*r_fork = philo->id % (*events)->philo_num;
	pthread_mutex_lock(&philo->philo_lock);
	philo->last_meal_time = curr_time();
	pthread_mutex_unlock(&philo->philo_lock);
	print_action(*events, philo->id, "is thinking");
	if (philo->id % 2 == 0)
		ft_usleep((*events)->time_to_eat / 2);
}

int	take_forks(t_events *events, int l_fork, int r_fork, t_philo *philo)
{
	if (l_fork < r_fork)
	{
		pthread_mutex_lock(events->forks[l_fork].lock_fork);
		if (if_ended(events))
			return (pthread_mutex_unlock(events->forks[l_fork].lock_fork), 1);
		print_action(events, philo->id, "has taken a fork");
		pthread_mutex_lock(events->forks[r_fork].lock_fork);
	}
	else
	{
		pthread_mutex_lock(events->forks[r_fork].lock_fork);
		if (if_ended(events))
			return (pthread_mutex_unlock(events->forks[r_fork].lock_fork), 1);
		print_action(events, philo->id, "has taken a fork");
		pthread_mutex_lock(events->forks[l_fork].lock_fork);
	}
	if (if_ended(events))
	{
		pthread_mutex_unlock(events->forks[l_fork].lock_fork);
		pthread_mutex_unlock(events->forks[r_fork].lock_fork);
		return (1);
	}
	print_action(events, philo->id, "has taken a fork");
	return (0);
}

void	release_forks(t_events *events, int l_fork, int r_fork)
{
	pthread_mutex_unlock(events->forks[r_fork].lock_fork);
	pthread_mutex_unlock(events->forks[l_fork].lock_fork);
}

void	eat(t_philo *philo, t_events *events)
{
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
}

void	sleep_and_think(t_philo *philo, t_events *events)
{
	print_action(events, philo->id, "is sleeping");
	ft_usleep(events->time_to_sleep);
	if (!if_ended(events))
		print_action(events, philo->id, "is thinking");
}
