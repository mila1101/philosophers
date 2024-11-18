/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoklova <msoklova@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 17:48:29 by msoklova          #+#    #+#             */
/*   Updated: 2024/11/15 15:28:42 by msoklova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	curr_time(void)
{
	static long long	start = -1;
	struct timeval		tv;
	long long				curr_time;

	gettimeofday(&tv, NULL);
	curr_time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	if (start == -1)
		start = curr_time;
	curr_time -= start;
	return ((curr_time / 100) * 100);
}

void	print_action(t_events *events, int id, const char *action)
{
	pthread_mutex_lock(&events->print_lock);
	printf("%lld %d %s\n", curr_time(),  id, action);
	pthread_mutex_unlock(&events->print_lock);
}