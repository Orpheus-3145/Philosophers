/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fra <fra@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 13:57:37 by fra               #+#    #+#             */
/*   Updated: 2023/05/06 19:24:24 by fra              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_BONUS_H
# define PHILOSOPHERS_BONUS_H
# define ALIVE_STATUS 		100
# define DEATH_STATUS 		101
# define FULLY_ATE_STATUS 	102
# define ERROR_STATUS 		103
# define RED   		"\x1B[31m"
# define GRN   		"\x1B[32m"
# define YEL   		"\x1B[33m"
# define BLU   		"\x1B[34m"
# define MAG   		"\x1B[35m"
# define CYN   		"\x1B[36m"
# define WHT   		"\x1B[37m"
# define RESET 		"\x1B[0m"
# include <string.h>    	// memset()
# include <stdlib.h>    	// malloc(), free()
# include <unistd.h>    	// write(), usleep(), fork()
# include <stdio.h>     	// printf()
# include <sys/time.h>		// gettimeofday()
# include <sys/wait.h>		// waitpid()
# include <signal.h>		// kill()
# include <fcntl.h>			// macro to open files
# include <pthread.h>		// thread functions
# include <semaphore.h>		// sem_****()
# include <stdbool.h>		// boolean types
# include <stdint.h>		// cross-compiler types

// represents action that the philo is currently doing
typedef enum s_action
{
	EAT,
	SLEEP,
	THINK,
	FORK,
	DIE
}	t_action;

// shortcut in naming variable
typedef struct timeval	t_timeval;

// struct that represents a philo [ '{!}' means that this value is the same for every philo]
// @param *time_sem			-> binary semaphore that protects the variable last_time_eat
// @param *status_sem		-> binary semaphore that protects the variable status
// @param *forks_sem		-> reference to the forks semaphore {!}
// @param *print_sem		-> reference to printer semaphore {!}
// @param phil_process		-> process of the philo (the function eat_sleep_repeat() is executed)
// @param check_death		-> thread that checks phil's status (the function monitoring() is executed)
// @param last_time_eat		-> timestamp of the last time the philo ate
// @param *_start_sim		-> reference of the start of the simulation {!}
// @param status_sem_name	-> name of philos's status_semaphore (see create_name_sem())
// @param time_sem_name		-> name of philos's time_semaphore (see create_name_sem())
// @param *print_sem		-> reference to printer semaphore {!}
// @param id				-> id of the philo (starting from 1)
// @param t_death			-> amount of millisecs the philo is able to survive without eating {!}
// @param t_eat				-> amount of millisecs the philo needs to eat {!}
// @param t_sleep			-> amount of millisecs the philo needs to sleep {!}
// @param status			-> current status of the philo
// @param meals				-> (optional, -1 if not given) amount of meals the philo has to eat before exiting the simulation {!}
typedef struct s_philo
{
	sem_t		*time_sem;
	sem_t		*status_sem;
	sem_t		*forks_sem;
	sem_t		*print_sem;
	pid_t		phil_process;
	pthread_t	check_death;
	t_timeval	last_time_eat;
	t_timeval	*_start_sim;
	char		*status_sem_name;
	char		*time_sem_name;
	uint32_t	id;
	uint32_t	t_death;
	uint32_t	t_eat;
	uint32_t	t_sleep;
	int32_t		status;
	int32_t		meals;
}	t_philo;

// deposit/container struct
// @param forks_sem			-> semaphore (cardinality=n_philos) that represents the amount of forks in the middle of the table
// @param print_sem			-> binary semaphore that allows to print messages in stdout
// @param philos			-> array of the philosophers
// @param n_philos			-> number of the philosophers
// @param _start_sim		-> global start of the simulation
typedef struct s_deposit
{
	sem_t		*forks_sem;
	sem_t		*print_sem;
	t_philo		*philos;
	uint32_t	n_philos;
	t_timeval	_start_sim;
}	t_deposit;

// constructor for the container struct
// @param argc	-> argc standard parameter
// @param argv	-> argv standard parameter
// @param depo	-> address to store the informations,
//
// return bool	-> false in case of errors (malloc(), new_sem() and create_philo() could fail)
bool		create_depo(int32_t argc, char **argv, t_deposit *depo);

// constructor for the philosopher struct
// @param phil	-> the instance of the philo to initialize
// @param argc	-> argc standard parameter
// @param argv	-> argv standard parameter [expected: n of philos, time death, time eat, 
//					time sleep [, max amount of meals to eat]]
// @param id	-> counter that represents the id of the philo
//
// return bool	-> false in case of errors (new_sem() and create_name_sem() could fail)
bool		create_philo(t_philo *phil, int32_t argc, char **argv, uint32_t id);

// beacuse every semaphore must have a unique name, the names of the sems
// for the time_sem and status_sem of every philos are are created with the
// format: "/[status - time]_semaphore_[ID_philo]"
// NB could return NULL in case the malloc fails
// @param id		-> id of the philo (variable part of the string)
// @param fixed		-> fixed part of the  string
//
// @return char*	-> new unique sem name
char		*create_name_sem(uint32_t id, char	*fixed);

// creates a new semaphore
// @param sem_name	-> name of the new semaphore
// @param semaphore	-> instance of the new semaphore
// @param amount	-> possible values of the semaphore
//
// return bool	-> false in case of errors (sem_open() could fail)
bool		new_sem(char *sem_name, sem_t *semaphore, int32_t amount);

// destructor of the container, calls free(), sem_close( and sem_unlink()
// @param depo	-> instance of the container to destroy
//
// return void
void		free_depo(t_deposit *depo);

// create children processes: for every philo a process is created
// after that if waits the termination of every philo, if the exit status is
// DEATH_STATUS it sends a SIG_INT to every child process
// NB: wait/post semaphore phil.time_sem (set_last_time_eat() and ft_msleep())
// @param depo	-> container struct
//
// @return void
void		begin_simulation(t_deposit *depo);

// first the monitor thread is created and detached, then the philo starts his
// usual cycle
// NB: wait/post semaphore phil.forks_sem
// NB: wait/post semaphore phil.time_semaphore (set_last_time_eat())
// NB: wait/post semaphore phil.status_semaphore (get_status(), print_message())
// @param phil	-> the single philosopher
//
// @return void
void		eat_sleep_repeat(t_philo *phil);

// this threads checks only his relative philosopher, and furthermore, only
// the death condition is verified, if philo dies the process is terminated
// @param param	-> the philosopher to check
//
// @return void*	-> it's always a NULL pointer
void		*monitoring(void *param);

// prints the current action of philo, if the status of the philo is
// different from ALIVE it returns without printing anything
// NB: wait/post semaphore phil.status_semaphore (get_status())
// NB: wait/post semaphore phil.print_semaphore
// @param philo		-> philo that does the action
// @param action	-> type of action (enum type)
//
// @return void
void		print_message(t_philo *philo, t_action action);

// gives back the current status of the philo
// NB: wait/post semaphore phil.status_sem 
// @param phil		-> philo to inspect
//
// return int32_t	-> current status
int32_t		get_status(t_philo *phil);

// sets the new status of the philo
// NB: wait/post semaphore phil.status_sem
// @param phil	-> philo to update
// @param value	-> new action to set
//
// return void
void		set_status(t_philo *phil, int32_t value);

// sets the new last time phil ate
// NB: wait/post semaphore phil.time_sem
// @param phil	-> philo to update
//
// @return void
void		set_last_time_eat(t_philo *phil);

// checks if the input is formatted correctly: only 4 or 5 non negative integers will be accepted,
// first [and optional last argument] must be strictly positive
// @param argc	-> argc standard parameter
// @param argv	-> argv standard parameter
//
// @return bool	-> input is valid or not
bool		check_input(int32_t argc, char **argv);

// stops the execution because of some error/anomaly
// @param depo		-> container to free
// @param message	-> message to display before killing the program
//
// @return void
void		kill_program(t_deposit *depo, char *message);

// converts a string of chars into an unsigned integer,
// NB does not check if the number is greater than MAX_UINT!
// @param str		-> sequence of chars to convert
//
// @return uint32_t	-> number obtained
uint32_t	ft_atoui(const char *str);

// finds the amount of milliseconds passed from t1 to t2
// NB: to avoid overflows t2 must happen not more than
// 35 minutes (2046 seconds, 2146483647 millisecs) after t1
// @param t1		-> start time
// @param t2		-> ending time
//
// @return int32_t	-> difference in milliseconds
int32_t		delta_time(t_timeval t1, t_timeval t2);

// stops the execution of the process of the philo for an amount of
// milliseconds, every 50 microseconds it checks if the philo is dead
// if so, exits from the function without further waiting
// NB: wait/post semaphore phil.status_semaphore (get_status())
// @param	phil		-> philo that is going to stop/sleep
// @param	milli_secs	-> amount of milliseconds to stop the execution
//
// @return void
void		ft_msleep(t_philo *phil, uint32_t milli_secs);


#endif