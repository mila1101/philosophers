/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoklova <msoklova@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 17:48:29 by msoklova          #+#    #+#             */
/*   Updated: 2024/11/20 17:32:16 by msoklova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	curr_time(void)
{
	struct timeval  tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int			ft_usleep(useconds_t time)
{
	long long	start;

	start = curr_time();
	while ((curr_time() - start) < time / 2)
		usleep(50);
	return (0);
}

void	print_action(t_events *events, int id, const char *action)
{
	pthread_mutex_lock(&events->print_lock);
	printf("%lld %d %s\n", curr_time() - events->start,  id, action);
	pthread_mutex_unlock(&events->print_lock);
}