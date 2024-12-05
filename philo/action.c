/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoklova <msoklova@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 13:55:46 by msoklova          #+#    #+#             */
/*   Updated: 2024/12/05 10:34:24 by msoklova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	one_philo(t_events *events, int l_fork, t_philo *philo)
{
	pthread_mutex_lock(events->forks[l_fork].lock_fork);
	print_action(events, philo->id, "has taken a fork");
	ft_usleep(events->time_to_die);
	pthread_mutex_unlock(events->forks[l_fork].lock_fork);
	return (1);
}

int	take_forks(t_events *events, int l_fork, int r_fork, t_philo *philo)
{
	if (events->philo_num == 1)
		return (one_philo(events, l_fork, philo));
	pthread_mutex_lock(events->forks[l_fork].lock_fork);
	if (events->dead)
	{
		pthread_mutex_unlock(events->forks[l_fork].lock_fork);
		return (1);
	}
	print_action(events, philo->id, "has taken a fork");
	pthread_mutex_lock(events->forks[r_fork].lock_fork);
	if (events->dead)
	{
		pthread_mutex_unlock(events->forks[l_fork].lock_fork);
		pthread_mutex_unlock(events->forks[r_fork].lock_fork);
		return (1);
	}
	print_action(events, philo->id, "has taken a fork");
	return (0);
}

void	eat(t_philo *philo, t_events *events, int l_fork, int r_fork)
{
	pthread_mutex_lock(&philo->philo_lock);
	if (if_ended(events))
	{
		pthread_mutex_unlock(events->forks[r_fork].lock_fork);
		pthread_mutex_unlock(events->forks[l_fork].lock_fork);
		pthread_mutex_unlock(&philo->philo_lock);
		return ;
	}
	philo->last_meal_time = curr_time();
	print_action(events, philo->id, "is eating");
	philo->meals_eaten++;
	if (philo->meals_eaten == events->meals_needed)
	{
		pthread_mutex_lock(&events->meal_lock);
		events->eaten++;
		pthread_mutex_unlock(&events->meal_lock);
	}
	pthread_mutex_unlock(&philo->philo_lock);
	ft_usleep(events->time_to_eat);
}

void	sleep_and_think(t_philo *philo, t_events *events)
{
	if (if_ended(events))
		return ;
	print_action(events, philo->id, "is sleeping");
	ft_usleep(events->time_to_sleep);
	if (!if_ended(events))
		print_action(events, philo->id, "is thinking");
}
