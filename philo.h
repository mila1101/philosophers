/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoklova <msoklova@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 14:29:50 by msoklova          #+#    #+#             */
/*   Updated: 2024/12/02 12:48:56 by msoklova         ###   ########.fr       */
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

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long			last_meal_time;
	pthread_mutex_t	philo_lock;
	pthread_t		thread;
	struct s_events	*events;
}					t_philo;

typedef struct s_forks
{
	int				taken;
	pthread_mutex_t	*lock_fork;
}					t_forks;

typedef struct s_events
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
void				init_philo(t_events *events);
t_events			*init_event_values(char **argv);
t_events			*init_events(char **argv);
t_events			*cleanup_events(t_events *events);
void				*death_monitor(void *arg);

/* ACTION */
void				i_philo(t_philo *philo, t_events **events, int *l_fork, int *r_fork);
int					take_forks(t_events *events, int l_fork, int r_fork, t_philo *philo);
void				release_forks(t_events *events, int l_fork, int r_fork);
void				release_forks2(t_events *events, int l_fork, int r_fork);
void				eat(t_philo *philo, t_events *events);
void				sleep_and_think(t_philo *philo, t_events *events);

/* UTILS */
int					ft_atoi(const char *str);
int					if_ended(t_events *events);
long				curr_time(void);
void				print_action(t_events *events, int id, const char *action);
int					ft_usleep(useconds_t time);
int					ft_isdigit(char *str);
int					check_input(char **argv);

#endif