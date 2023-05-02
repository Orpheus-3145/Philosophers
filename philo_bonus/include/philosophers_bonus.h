/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers_bonus.h                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: fra <fra@student.42.fr>                      +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/23 13:57:37 by fra           #+#    #+#                 */
/*   Updated: 2023/04/30 18:59:12 by faru          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_BONUS_H
# define PHILOSOPHERS_BONUS_H
# define ALIVE_STATUS 100
# define DEATH_STATUS 101
# define FULLY_ATE_STATUS 102
# define ERROR_STATUS 103
# define RED   "\x1B[31m"
# define GRN   "\x1B[32m"
# define YEL   "\x1B[33m"
# define BLU   "\x1B[34m"
# define MAG   "\x1B[35m"
# define CYN   "\x1B[36m"
# define WHT   "\x1B[37m"
# define RESET "\x1B[0m"
# include <string.h>    		// memset()
# include <stdlib.h>    		// malloc(), free()
# include <unistd.h>    		// write(), usleep(), fork()
# include <stdio.h>     		// printf()
# include <sys/time.h>			// gettimeofday()
# include <sys/wait.h>			// waitpid()
# include <signal.h>			// kill()
# include <fcntl.h>				// macro to open files
# include <pthread.h>			// thread functions
# include <semaphore.h>			// sem_****()
# include <stdbool.h>			// boolean types
# include <stdint.h>			// cross-compiler types

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
	sem_t		*forks_sem;
	sem_t		*print_sem;
	sem_t		*status_sem;
	char		*st_sem_name;
	sem_t		*time_sem;
	char		*tm_sem_name;
	pid_t		phil_process;
	pthread_t	check_death;
	t_timeval	last_time_eat;
	t_timeval	*_start_sim;
	int32_t		status;
	int32_t		meals;
	uint32_t	id;
	uint32_t	t_death;
	uint32_t	t_eat;
	uint32_t	t_sleep;
}	t_philo;

typedef struct s_deposit
{
	sem_t		*forks_sem;
	sem_t		*print_sem;
	t_philo		*philos;
	t_timeval	_start_sim;
	uint32_t	n_philos;
}	t_deposit;

bool		create_depo(int32_t argc, char **argv, t_deposit *depo);

bool		create_philo(t_philo *phil, int32_t argc, char **argv, uint32_t id);

bool		new_sem(char *sem_name, sem_t *semaphore, int32_t amount);

void		free_depo(t_deposit *depo);

int32_t		get_status(t_philo *phil);

void		set_status(t_philo *phil, int32_t value);

void		set_last_time_eat(t_philo *phil);

void		begin_simulation(t_deposit *depo);

void		eat_sleep_repeat(t_philo *phil);

void		*monitoring(void *param);

void		print_message(t_philo *philo, t_action action);

bool		check_input(int32_t argc, char **argv);

uint32_t	ft_atoui(const char *str);

int32_t		delta_time(t_timeval t1, t_timeval t2);

void		ft_usleep(t_philo *phil, uint32_t milli_secs);

void		kill_program(t_deposit *depo, char *message);

void		terminate_procs(t_deposit *depo);

char		*create_name_sem(uint32_t id, char	*fixed);

void		open_sems(t_philo *phil);

void		close_sems(t_philo *phil);

#endif