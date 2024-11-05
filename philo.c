/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoklova <msoklova@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 15:39:41 by msoklova          #+#    #+#             */
/*   Updated: 2024/11/05 10:54:10 by msoklova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_events	*init_events(char **argv)
{
	t_events	*events;
	int			i;

	events = malloc(sizeof(t_events));
	if (!events)
		return (NULL);
	events->philo_num = ft_atoi(argv[1]);
	events->time_to_die = ft_atoi(argv[2]);
	events->time_to_eat = ft_atoi(argv[3]);
	events->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		events->meals_needed = ft_atoi(argv[5]);
	else
		events->meals_needed = -1;
	events->eaten = INT_MAX;
	events->dead = 0;
	events->philo = malloc(sizeof(t_philo) * events->philo_num);
	events->forks = malloc(sizeof(t_forks) * events->philo_num);
	if (!events->philo || events->forks)
	{
		return (NULL);
	}
	pthread_mutex_init(&events->print_lock, NULL);
	init_philo(events);
	i = 0;
	while (i < events->philo_num)
	{
		events->forks[i].taken = 0;
		events->forks[i].lock_fork = malloc(sizeof(pthread_mutex_t));

		i++;
	}
	return (events);
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
	while (!events->dead)
	{
		print_action(events, philo->id, "is thinking");
		pthread_mutex_lock(events->forks[l_fork].lock_fork);
		print_action(events, philo->id, "has taken a fork");
		pthread_mutex_lock(events->forks[r_fork].lock_fork);
		print_action(events, philo->id, "has taken a fork");

		// Eating
		pthread_mutex_lock(&philo->philo_lock);
		print_action(events, philo->id, "is eating");
		philo->last_meal_time = curr_time();
		philo->meals_eaten++;
		pthread_mutex_unlock(&philo->philo_lock);

		usleep(events->time_to_eat * 1000);  // Eating duration

		// Put down forks
		pthread_mutex_unlock(events->forks[l_fork].lock_fork);
		pthread_mutex_unlock(events->forks[r_fork].lock_fork);

		// Sleeping
		print_action(events, philo->id, "is sleeping");
		usleep(events->time_to_sleep * 1000);
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_events	*events;
	int			i;

	//needs error handling here
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
	i = 0;
	while (i < events->philo_num)
	{
		pthread_join(events->philo[i].thread, NULL);
		i++;
	}
	return (0);
}