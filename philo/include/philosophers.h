/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fra <fra@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 13:57:37 by fra               #+#    #+#             */
/*   Updated: 2023/05/06 19:19:50 by fra              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# define RED   "\x1B[31m"	// colors for the messages on the stdout
# define GRN   "\x1B[32m"
# define YEL   "\x1B[33m"
# define BLU   "\x1B[34m"
# define MAG   "\x1B[35m"
# define RESET "\x1B[0m"
# include <string.h>    	// memset()
# include <stdlib.h>    	// malloc(), free()
# include <unistd.h>    	// write(), usleep()
# include <stdio.h>     	// printf()
# include <sys/time.h>		// gettimeofday()
# include <fcntl.h>			// macro to open files (O_CREAT, O_WRONLY, O_RDONLY ..)
# include <pthread.h>		// thread/mutex functions
# include <stdbool.h>		// boolean types
# include <stdint.h>		// cross-compiler types

// represents the possible status the single philo
// can assume
typedef enum s_status
{
	ALIVE,
	FULLY_ATE,
	DEAD,
	ERROR
}	t_status;

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
// @param time_mutex	-> mutex that locks the variable last_time_eat
// @param status_mutex	-> mutex that locks the variable status
// @param r_fork		-> mutex that represents the fork (the variable to lock is the mutex itself)
// @param *l_fork		-> reference to r_fork of the previous philospher
// @param phil_thread	-> thread of the philo (the function eat_sleep_repeat() is executed)
// @param last_time_eat	-> timestamp of the last time the philo ate
// @param *_start_sim	-> reference of the start of the simulation {!}
// @param status		-> current status of the philo (enum type)
// @param id			-> id of the philo (starting from 1)
// @param t_death		-> amount of millisecs the philo is able to survive without eating {!}
// @param t_eat			-> amount of millisecs the philo needs to eat {!}
// @param t_sleep		-> amount of millisecs the philo needs to sleep {!}
// @param meals			-> (optional, -1 if not given) amount of meals the philo has to eat before exiting the simulation {!}
typedef struct s_philo
{
	pthread_mutex_t	time_mutex;
	pthread_mutex_t	status_mutex;
	pthread_mutex_t	r_fork;
	pthread_mutex_t	*l_fork;
	pthread_t		phil_thread;
	t_timeval		last_time_eat;
	t_timeval		*_start_sim;
	t_status		status;
	uint32_t		id;
	uint32_t		t_death;
	uint32_t		t_eat;
	uint32_t		t_sleep;
	int32_t			meals;
}	t_philo;

// deposit/container struct
// @param philos			-> array of the philosophers
// @param n_philos			-> number of the philosophers
// @param _start_sim		-> global start of the simulation
// @param philos_fully_ate	-> in case the 5th argument is given, tells if every philo ate enough times
typedef struct s_deposit
{
	t_philo			*philos;
	uint32_t		n_philos;
	t_timeval		_start_sim;
	bool			philos_fully_ate;
}	t_deposit;

// constructor for the container struct
// @param argc	-> argc standard parameter
// @param argv	-> argv standard parameter
// @param depo	-> address to store the informations,
//
// return bool	-> false in case of errors (malloc() and create_philo() could fail)
bool		create_depo(int32_t argc, char **argv, t_deposit *depo);

// constructor for the philosopher struct
// @param phil	-> the instance of the philo to initialize
// @param argc	-> argc standard parameter
// @param argv	-> argv standard parameter [expected: n of philos, time death, time eat, time sleep [, max amount of meals to eat]]
// @param id	-> counter that represents the id of the philo
//
// return bool	-> false in case of errors (pthread_mutex_init() could fail)
bool		create_philo(t_philo *phil, int32_t argc, char **argv, uint32_t id);

// destructor of the container, calls free() and pthread_mutex_destroy()
// @param depo	-> instance of the container to destroy
//
// return void
void		free_depo(t_deposit *depo);

// gives back the current status of the philo
// NB: locks/unlocks mutex phil.status_mutex 
// @param phil		-> philo to inspect
//
// return t_status	-> current status
t_status	get_status(t_philo *phil);

// sets the new status of the philo
// NB: locks/unlocks mutex phil.status_mutex
// @param phil	-> philo to update
// @param value	-> new action to set
//
// return void
void		set_status(t_philo *phil, t_status value);

// sets the new last time phil ate
// NB: locks/unlocks mutex phil.time_mutex
// @param phil	-> philo to update
//
// @return void
void		set_last_time_eat(t_philo *phil);

// create children threads: one for every philo and another one to check every philo
// NB: locks/unlocks mutex phil.time_mutex (set_last_time_eat())
// @param depo	-> container struct
//
// @return void
void		begin_simulation(t_deposit *depo);

// function executed for every philo-thread, lets them eat, sleep and think, and occasionally die
// in case of error while creating/joining threads (functions pthread_create/_join() could fail)
// the function kill_program is called and the execution stops
// NB: locks/unlocks mutex phil.r_fork
// NB: locks/unlocks mutex phil.l_fork
// NB: locks/unlocks mutex phil.time_mutex (set_last_time_eat())
// NB: locks/unlocks mutex phil.status_mutex (get_status(), print_message())
// @param param		-> the single philosopher
//
// @return void*	-> it's always a NULL pointer
void		*eat_sleep_repeat(void *param);

// thread that loops the array of philosophers and for every one them check the two possible
// exit conditions:
//		1. philo is dead for hunger
//		2. ALL the philosopher ate max_meals time
// if the exit condition is the fisrt one, it aslo sets status=DEAD for every philo
// NB: locks/unlocks mutex phil.status_mutex (check_status(), set_status())
// NB: locks/unlocks mutex phil.time_mutex (is_dead())
// @param param	->
//
// @return void*	-> it's always a NULL pointer
void		*monitoring(void *param);

// checks if the difference between the current time (before locking the mutex) and 
// the last time phil ate (philo.last_time_eat) is greater than philo.t_death then phil dies
// (and all the others aswell)
// NB: locks/unlocks mutex phil.time_mutex
// @param phil	-> philo to inspect
//
// @return bool	-> true(ALIVE)/false(DEAD)
bool		is_dead(t_philo *phil);

// prints the current action of philo, if the status of the philo is
// different from ALIVE it returns without printing anything
// NB: locks/unlocks mutex phil.status_mutex (check_status())
// @param philo		-> philo that does the action
// @param action	-> type of action (enum type)
//
// @return void
void		print_message(t_philo *philo, t_action action);

// checks if the input is formatted correctly: only 4 or 5 non negative integers will be accepted,
// first [and optional last argument] must be strictly positive
// @param argc	-> argc standard parameter
// @param argv	-> argv standard parameter
//
// @return bool	-> input is valid or not
bool		check_input(int32_t argc, char **argv);

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

// stops the execution of the thread of the philo for an amount of
// milliseconds, every 50 microseconds it checks if the philo is dead
// if so, exits from the function without further waiting
// NB: locks/unlocks mutex phil.status_mutex (check_status())
// @param	phil		-> philo that is going to stop/sleep
// @param	milli_secs	-> amount of milliseconds to stop the execution
//
// @return void
void		ft_msleep(t_philo *phil, uint32_t milli_secs);

// stops the execution of the main thread because of some error/anomaly
// NB: locks/unlocks mutex phil.status_mutex (set_status())
// @param depo		-> container to free
// @param message	-> message to display before killing the program
// @param terminate	-> if other threads are still running, 
//					   sets status=ERROR for every philo, so the thread can terminate
//
// @return void
void		kill_program(t_deposit *depo, char *message, bool terminate);

#endif