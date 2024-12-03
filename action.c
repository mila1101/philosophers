/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoklova <msoklova@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 13:55:46 by msoklova          #+#    #+#             */
/*   Updated: 2024/12/03 13:38:43 by msoklova         ###   ########.fr       */
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
	if (l_fork < r_fork)
	{
		pthread_mutex_lock(events->forks[l_fork].lock_fork);
		if (if_ended(events))
			return (pthread_mutex_unlock(events->forks[l_fork].lock_fork), 1);
		print_action(events, philo->id, "has taken a fork");
		pthread_mutex_lock(events->forks[r_fork].lock_fork);
		if (if_ended(events))
			return (release_forks2(events, l_fork, r_fork), 1);
	}
	else
	{
		pthread_mutex_lock(events->forks[r_fork].lock_fork);
		if (if_ended(events))
			return (pthread_mutex_unlock(events->forks[r_fork].lock_fork), 1);
		print_action(events, philo->id, "has taken a fork");
		pthread_mutex_lock(events->forks[l_fork].lock_fork);
		if (if_ended(events))
			return (release_forks2(events, l_fork, r_fork), 1);
	}
	print_action(events, philo->id, "has taken a fork");
	return (0);
}

void	release_forks2(t_events *events, int l_fork, int r_fork)
{
	pthread_mutex_unlock(events->forks[l_fork].lock_fork);
	pthread_mutex_unlock(events->forks[r_fork].lock_fork);
}

void	release_forks(t_events *events, int l_fork, int r_fork)
{
	pthread_mutex_unlock(events->forks[r_fork].lock_fork);
	pthread_mutex_unlock(events->forks[l_fork].lock_fork);
}

void	eat(t_philo *philo, t_events *events)
{
	if (!if_ended(events))
		return ;
	pthread_mutex_lock(&philo->philo_lock);
	philo->last_meal_time = curr_time();
	print_action(events, philo->id, "is eating");
	ft_usleep(events->time_to_eat);
	philo->meals_eaten++;
	if (philo->meals_eaten == events->meals_needed)
	{
		pthread_mutex_lock(&events->meal_lock);
		events->eaten++;
		pthread_mutex_unlock(&events->meal_lock);
	}
	pthread_mutex_unlock(&philo->philo_lock);
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
