/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoklova <msoklova@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 15:39:41 by msoklova          #+#    #+#             */
/*   Updated: 2024/11/02 16:46:01 by msoklova         ###   ########.fr       */
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
	//probably some other things too
}

int	main(int argc, char **argv)
{
	t_events	*events;

	//needs error handling here
	events = init_events(argv);
}