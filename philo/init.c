/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoklova <msoklova@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 16:01:40 by msoklova          #+#    #+#             */
/*   Updated: 2024/12/05 10:35:25 by msoklova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_philo(t_events *events)
{
	int	i;

	events->start = curr_time();
	i = 0;
	while (i < events->philo_num)
	{
		events->philo[i].id = i + 1;
		events->philo[i].meals_eaten = 0;
		events->philo[i].events = events;
		pthread_mutex_init(&events->philo[i].philo_lock, NULL);
		i++;
	}
}

t_events	*init_event_values(char **argv)
{
	t_events	*events;

	events = malloc(sizeof(t_events));
	if (!events)
		return (NULL);
	if (check_input(argv))
		return (NULL);
	events->philo_num = ft_atoi(argv[1]);
	events->time_to_die = ft_atoi(argv[2]);
	events->time_to_eat = ft_atoi(argv[3]);
	events->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		events->meals_needed = ft_atoi(argv[5]);
	else
		events->meals_needed = -1;
	events->eaten = 0;
	pthread_mutex_init(&events->meal_lock, NULL);
	events->dead = 0;
	events->starter = 0;
	return (events);
}

t_events	*init_events(char **argv)
{
	t_events	*events;
	int			i;

	events = init_event_values(argv);
	if (!events)
		return (NULL);
	events->philo = malloc(sizeof(t_philo) * events->philo_num);
	events->forks = malloc(sizeof(t_forks) * events->philo_num);
	if (!events->philo || !events->forks)
		return (cleanup_events(events));
	pthread_mutex_init(&events->print_lock, NULL);
	events->dead_mutex = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(events->dead_mutex, NULL);
	init_philo(events);
	i = 0;
	while (i < events->philo_num)
	{
		events->forks[i].taken = 0;
		events->forks[i].lock_fork = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(events->forks[i].lock_fork, NULL);
		i++;
	}
	return (events);
}

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

t_events	*cleanup_events(t_events *events)
{
	if (!events)
		return (NULL);
	free(events->philo);
	free(events->forks);
	free(events);
	return (NULL);
}
