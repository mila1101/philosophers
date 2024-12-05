/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoklova <msoklova@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 16:21:01 by msoklova          #+#    #+#             */
/*   Updated: 2024/12/05 10:42:24 by msoklova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	num;

	i = 0;
	sign = 1;
	num = 0;
	while ((str[i] >= '\t' && str[i] <= '\r') || str[i] == ' ')
		i++;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		num = (num * 10) + (str[i] - '0');
		i++;
	}
	return (num * sign);
}

int	ft_isdigit(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	check_input(char **argv)
{
	int	i;
	int	num;

	i = 1;
	while (argv[i])
	{
		num = ft_atoi(argv[i]);
		if (num < 1 || num > INT_MAX || (!ft_isdigit(argv[i]))
			|| ft_atoi(argv[1]) == 0)
		{
			printf("Wrong input!\n");
			return (1);
		}
		i++;
	}
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
