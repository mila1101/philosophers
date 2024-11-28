/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoklova <msoklova@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 15:39:41 by msoklova          #+#    #+#             */
/*   Updated: 2024/11/28 13:26:05 by msoklova         ###   ########.fr       */
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
	events->eaten = 0;
	pthread_mutex_init(&events->meal_lock, NULL);
	events->dead = 0;
	events->philo = malloc(sizeof(t_philo) * events->philo_num);
	events->forks = malloc(sizeof(t_forks) * events->philo_num);
	if (!events->philo || !events->forks)
	{
		free(events->philo);
		free(events->forks);
		free(events);
		return (NULL);
	}
	pthread_mutex_init(&events->print_lock, NULL);
	events->dead_mutex = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(events->dead_mutex, NULL);
	init_philo(events);
	i = 0;
	events->start = curr_time();
	while (i < events->philo_num)
	{
		events->forks[i].taken = 0;
		events->forks[i].lock_fork = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(events->forks[i].lock_fork, NULL);
		i++;
	}
	return (events);
}

int	if_ended(t_events *events)
{
	pthread_mutex_lock(events->dead_mutex);
	int ended = events->dead;
	pthread_mutex_unlock(events->dead_mutex);
	return ended;
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
	philo->last_meal_time = curr_time();

	print_action(events, philo->id, "is thinking");
	if (philo->id % 2 == 0)
		ft_usleep(events->time_to_eat / 2);
	while (!events->dead)
	{
		pthread_mutex_lock(events->dead_mutex);
		if (events->dead || (events->meals_needed != -1 && philo->meals_eaten >= events->meals_needed))
		{
			pthread_mutex_unlock(events->dead_mutex);
			break ;
		}
		pthread_mutex_unlock(events->dead_mutex);
		pthread_mutex_lock(events->forks[l_fork].lock_fork);
		if (events->dead)
		{
			pthread_mutex_unlock(events->forks[l_fork].lock_fork);
			break ;
		}
		print_action(events, philo->id, "has taken a fork");
		if (events->philo_num == 1)
		{
			ft_usleep(events->time_to_die);
			print_action(events, 1, "died");
			return (pthread_mutex_unlock(events->forks[l_fork].lock_fork), NULL);
		}
		pthread_mutex_lock(events->forks[r_fork].lock_fork);
		if (if_ended(events))
		{
			pthread_mutex_unlock(events->forks[l_fork].lock_fork);
			pthread_mutex_unlock(events->forks[r_fork].lock_fork);
			break ;
		}
		print_action(events, philo->id, "has taken a fork");
		pthread_mutex_lock(&philo->philo_lock);
		if (if_ended(events))
		{
			pthread_mutex_unlock(&philo->philo_lock);
			pthread_mutex_unlock(events->forks[l_fork].lock_fork);
			pthread_mutex_unlock(events->forks[r_fork].lock_fork);
			break ;
		}
		philo->last_meal_time = curr_time();
		print_action(events, philo->id, "is eating");
		philo->meals_eaten++;
		if (philo->meals_eaten == events->meals_needed)
		{
			pthread_mutex_lock(&events->meal_lock);
			events->eaten++;
			pthread_mutex_unlock(&events->meal_lock);
		}
		pthread_mutex_unlock(&philo->philo_lock);
		ft_usleep(events->time_to_eat);
		pthread_mutex_unlock(events->forks[l_fork].lock_fork);
		pthread_mutex_unlock(events->forks[r_fork].lock_fork);
		if (if_ended(events))
			break ;
		print_action(events, philo->id, "is sleeping");
		ft_usleep(events->time_to_sleep);
		if (if_ended(events))
			break ;
		print_action(events, philo->id, "is thinking");
	}
	return (NULL);
}

//void *routine(void *arg)
//{
//	t_philo		*philo;
//	t_events	*events;
//	int			l_fork;
//	int			r_fork;

//	philo = (t_philo *)arg;
//	events = philo->events;
//	l_fork = philo->id - 1;
//	r_fork = philo->id % events->philo_num;
//	philo->last_meal_time = curr_time();

//	print_action(events, philo->id, "is thinking");
//	if (philo->id % 2 == 0)
//		ft_usleep(events->time_to_eat / 2);
//	while (!events->dead)
//	{
//		if (events->meals_needed != -1 && philo->meals_eaten >= events->meals_needed)
//			break;
//		if (events->dead)
//			break ;
//		pthread_mutex_lock(events->forks[l_fork].lock_fork);
//		if (events->dead)
//		{
//			pthread_mutex_unlock(events->forks[l_fork].lock_fork);
//			break ;
//		}
//		print_action(events, philo->id, "has taken a fork");
//		if (events->philo_num == 1)
//		{
//			ft_usleep(events->time_to_die);
//			print_action(events, 1, "died");
//			return (pthread_mutex_unlock(events->forks[l_fork].lock_fork), NULL);
//		}
//		pthread_mutex_lock(events->forks[r_fork].lock_fork);
//		if (events->dead)
//		{
//			pthread_mutex_unlock(events->forks[l_fork].lock_fork);
//			pthread_mutex_unlock(events->forks[r_fork].lock_fork);
//			break ;
//		}
//		print_action(events, philo->id, "has taken a fork");
//		pthread_mutex_lock(&philo->philo_lock);
//		if (events->dead)
//		{
//			pthread_mutex_unlock(&philo->philo_lock);
//			pthread_mutex_unlock(events->forks[l_fork].lock_fork);
//			pthread_mutex_unlock(events->forks[r_fork].lock_fork);
//			break ;
//		}
//		philo->last_meal_time = curr_time();
//		print_action(events, philo->id, "is eating");
//		philo->meals_eaten++;
//		if (philo->meals_eaten == events->meals_needed)
//		{
//			pthread_mutex_lock(&events->meal_lock);
//			events->eaten++;
//			pthread_mutex_unlock(&events->meal_lock);
//		}
//		pthread_mutex_unlock(&philo->philo_lock);
//		ft_usleep(events->time_to_eat);
//		pthread_mutex_unlock(events->forks[l_fork].lock_fork);
//		pthread_mutex_unlock(events->forks[r_fork].lock_fork);
//		if (events->dead)
//			break ;
//		print_action(events, philo->id, "is sleeping");
//		ft_usleep(events->time_to_sleep);
//		if (events->dead)
//			break ;
//		print_action(events, philo->id, "is thinking");
//	}
//	return (NULL);
//}

int	main(int argc, char **argv)
{
	t_events	*events;
	pthread_t	monitor;
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
	pthread_mutex_init(&events->meal_lock, NULL);
	i = 0;
	//pthread_join(monitor, NULL);
	while (i < events->philo_num)
	{
		pthread_create(&events->philo[i].thread, NULL, routine, &events->philo[i]);
		i++;
	}
	pthread_create(&monitor, NULL, death_monitor, events);
	pthread_detach(monitor);
	i = 0;
	while (i < events->philo_num)
	{
		pthread_join(events->philo[i].thread, NULL);
		i++;
	}
	pthread_join(monitor, NULL);
	pthread_mutex_destroy(&events->meal_lock);
	return (0);
}

