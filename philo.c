/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoklova <msoklova@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 15:39:41 by msoklova          #+#    #+#             */
/*   Updated: 2024/11/03 16:08:18 by msoklova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_events	*init_events(char **argv)
{
	t_events	*events;

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
	if (!events->philo)
	{
		return (NULL);
	}
	pthread_mutex_init(&events->print_lock, NULL);
	init_philo(events);
	return (events);
}

int	main(int argc, char **argv)
{
	t_events	*events;

	//needs error handling here
	if (argc < 5 || argc > 6)
	{
		printf("Correct use: [philosophers] [death] [eat] [sleep] optional: [meals]\n");
		return (-1);
	}
	events = init_events(argv);
	return (0);
}