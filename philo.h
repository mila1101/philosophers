/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoklova <msoklova@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 14:29:50 by msoklova          #+#    #+#             */
/*   Updated: 2024/11/25 11:22:03 by msoklova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdio.h>
# include <stdint.h>
# include <stdlib.h>
# include <limits.h>

typedef struct	s_philo
{
	int				id;
	int				meals_eaten;
	long			last_meal_time;
	pthread_mutex_t	philo_lock;
	pthread_t		thread;
	struct s_events	*events;
}					t_philo;

typedef struct	s_forks
{
	int				taken;
	pthread_mutex_t	*lock_fork;
}					t_forks;

typedef struct	s_events
{
	int				philo_num;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meals_needed;
	int				eaten;
	long			start;
	int				dead;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	*dead_mutex;
	t_philo			*philo;
	t_forks			*forks;
}					t_events;

/* INIT */
void		init_philo(t_events *events);
t_events	*init_events(char **argv);
void		*death_monitor(void *arg);

/* UTILS */
int				ft_atoi(const char *str);
long			curr_time(void);
void			print_action(t_events *events, int id, const char *action);
int				ft_usleep(useconds_t time);
//int	undead(t_events *events);

#endif