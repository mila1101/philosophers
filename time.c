/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoklova <msoklova@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 17:48:29 by msoklova          #+#    #+#             */
/*   Updated: 2024/11/05 16:05:50 by msoklova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	curr_time(void)
{
	struct timeval	tv;
	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	print_action(t_events *events, int id, const char *action)
{
	pthread_mutex_lock(&events->print_lock);
	printf("[insert timestamp here] %d %s\n", id, action);
	pthread_mutex_unlock(&events->print_lock);
}