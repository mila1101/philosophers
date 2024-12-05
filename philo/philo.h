/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoklova <msoklova@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 14:29:50 by msoklova          #+#    #+#             */
/*   Updated: 2024/12/05 10:49:09 by msoklova         ###   ########.fr       */
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
	int				starter;
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
void				i_philo(t_philo *philo, t_events **events,
						int *l_fork, int *r_fork);

/* ROUTINE */
int					if_ended(t_events *events);
void				*routine(void *arg);

/* MONITOR */
void				*death_monitor(void *arg);

/* ACTION */
int					one_philo(t_events *events, int l_fork, t_philo *philo);
int					take_forks(t_events *events, int l_fork,
						int r_fork, t_philo *philo);
void				eat(t_philo *philo, t_events *events,
						int l_fork, int r_fork);
void				sleep_and_think(t_philo *philo, t_events *events);

/* PHILO */
int					check_args(int argc);
int					init_threads(t_events *events, pthread_t *monitor);
void				join_threads(t_events *events, pthread_t monitor);

/* TIME */
long				curr_time(void);
int					ft_usleep(useconds_t time);
void				print_action(t_events *events, int id, const char *action);

/* UTILS */
int					ft_atoi(const char *str);
int					ft_isdigit(char *str);
int					check_input(char **argv);
void				release_forks(t_events *events, int l_fork, int r_fork);
void				release_forks2(t_events *events, int l_fork, int r_fork);

#endif