/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoklova <msoklova@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 16:01:40 by msoklova          #+#    #+#             */
/*   Updated: 2024/11/02 16:14:19 by msoklova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_philo(t_events *events)
{
	int	i;

	i = 0;
	while(i < events->philo_num)
	{
		events->philo[i].id = i + 1;
		events->philo[i].meals_eaten = 0;
		events->philo[i].last_meal_time = 0;
		pthread_mutex_init(&events->philo[i].philo_lock, NULL);
		i++;
	}
}