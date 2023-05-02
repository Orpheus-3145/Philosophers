/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers.h                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: fra <fra@student.42.fr>                      +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/23 13:57:37 by fra           #+#    #+#                 */
/*   Updated: 2023/04/30 18:58:38 by faru          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <string.h>    // memset()
# include <stdlib.h>    // malloc(), free()
# include <unistd.h>    // write(), usleep()
# include <stdio.h>     // printf()
# include <sys/time.h>	// gettimeofday()
# include <fcntl.h>		// macro to open files (O_CREAT, O_WRONLY, O_RDONLY ..)
# include <pthread.h>	// thread/mutex functions
# include <stdbool.h>	// boolean types
# include <stdint.h>	// cross-compiler types
# define RED   "\x1B[31m"
# define GRN   "\x1B[32m"
# define YEL   "\x1B[33m"
# define BLU   "\x1B[34m"
# define MAG   "\x1B[35m"
# define CYN   "\x1B[36m"
# define WHT   "\x1B[37m"
# define RESET "\x1B[0m"

typedef enum s_status
{
	ALIVE,
	FULLY_ATE,
	DEAD,
	ERROR
}	t_status;

typedef enum s_action
{
	EAT,
	SLEEP,
	THINK,
	FORK,
	DIE
}	t_action;

typedef struct timeval	t_timeval;

typedef struct s_philo
{
	pthread_mutex_t	time_mutex;
	pthread_mutex_t	status_mutex;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	r_fork;
	pthread_t		phil_thread;
	int32_t			meals;
	uint32_t		id;
	uint32_t		t_death;
	uint32_t		t_eat;
	uint32_t		t_sleep;
	t_timeval		last_time_eat;
	t_timeval		start_sim;
	t_timeval		*_start_sim;
	t_status		status;
}	t_philo;

typedef struct s_deposit
{
	t_timeval		_start_sim;
	t_philo			*philos;
	uint32_t		n_philos;
	uint32_t		max_meals;
	bool			philos_fully_ate;
}	t_deposit;

bool		create_depo(int32_t argc, char **argv, t_deposit *depo);

bool		create_philo(t_philo *phil, int32_t argc, char **argv, uint32_t id);

void		free_depo(t_deposit *depo);

bool		check_input(int32_t argc, char **argv);

uint32_t	ft_atoui(const char *str);

int32_t		delta_time(t_timeval t1, t_timeval t2);

void		ft_usleep(t_philo *phil, uint32_t milli_secs);

void		kill_program(t_deposit *depo, char *message, bool terminate);

void		begin_simulation(t_deposit *depo);

void		*eat_sleep_repeat(void *param);

void		*monitoring(void *param);

bool		is_dead(t_philo *phil);

void		print_message(t_philo *philo, t_action action);

t_status	check_status(t_philo *phil);

void		set_status(t_philo *phil, t_status value);

void		set_last_time_eat(t_philo *phil);

#endif