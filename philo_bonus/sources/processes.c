/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fra <fra@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 19:04:02 by fra               #+#    #+#             */
/*   Updated: 2023/05/06 22:30:41 by fra              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void	begin_simulation(t_deposit *depo)
{
	t_philo		*phil;
	pid_t		terminated_pid;
	int32_t		status_procs;
	uint32_t	i;

	i = 0;
	while (i < depo->n_philos)
	{
		phil = depo->philos + i++;
		phil->phil_process = fork();
		if (phil->phil_process == -1)
			kill_program(depo, "program killed: error while creating process");
		else if (phil->phil_process == 0)
		{
			set_last_time_eat(phil);
			while (delta_time(*phil->_start_sim, phil->last_time_eat) < 0)
				set_last_time_eat(phil);
			// if (phil->id % 2 == 0)
			// 	ft_msleep(phil, 5);
			*phil->_start_sim = phil->last_time_eat;
			eat_sleep_repeat(phil);
		}
	}
	while (true)
	{
		terminated_pid = waitpid(-1, &status_procs, 0);
		if (terminated_pid <= 0)
			break ;
		else if (WIFEXITED(status_procs) && WEXITSTATUS(status_procs) != FULLY_ATE_STATUS)
		{
			phil = depo->philos;
			while (phil->phil_process != terminated_pid)
				phil++;
			i = 0;
			while (i < depo->n_philos)
				kill(depo->philos[i++].phil_process, SIGINT);
			if (WEXITSTATUS(status_procs) == ERROR_STATUS)
				kill_program(depo, "program killed: error in child thread");
			if (WEXITSTATUS(status_procs) == ALIVE_STATUS)
				kill_program(depo, "Phil is still alive: this should not be possible");
		}
	}
}

void	eat_sleep_repeat(t_philo *phil)
{
	phil->forks_sem = sem_open("/forks_semaphore", 0);
	phil->print_sem = sem_open("/print_semaphore", 0);
	phil->status_sem = sem_open(phil->status_sem_name, 0);
	phil->time_sem = sem_open(phil->time_sem_name, 0);
	if (pthread_create(&phil->check_death, NULL, &monitoring, phil))
		set_status(phil, ERROR_STATUS);
	else
		pthread_detach(phil->check_death);
	while (get_status(phil) == ALIVE_STATUS)
	{
		sem_wait(phil->forks_sem);
		print_message(phil, FORK);
		sem_wait(phil->forks_sem);
		print_message(phil, FORK);
		set_last_time_eat(phil);
		print_message(phil, EAT);
		ft_msleep(phil, phil->t_eat);
		sem_post(phil->forks_sem);
		sem_post(phil->forks_sem);
		if (phil->meals && ! --phil->meals)
			set_status(phil, FULLY_ATE_STATUS);
		print_message(phil, SLEEP);
		ft_msleep(phil, phil->t_sleep);
		print_message(phil, THINK);
	}
	sem_close(phil->forks_sem);
	sem_close(phil->print_sem);
	sem_close(phil->status_sem);
	sem_close(phil->time_sem);
	exit(get_status(phil));
}

void	*monitoring(void *param)
{
	t_philo		*phil;
	t_timeval	current;
	bool		status;

	phil = (t_philo *) param;
	while (true)
	{
		gettimeofday(&current, NULL);
		sem_wait(phil->time_sem);
		if (delta_time(phil->last_time_eat, current) >= (int32_t) phil->t_death)
			status = true;
		else
			status = false;
		sem_post(phil->time_sem);
		if (status)
			break ;
		else
			ft_msleep(phil, 10);
	}
	print_message(phil, DIE);
	set_status(phil, DEATH_STATUS);
	return (NULL);
}

void	print_message(t_philo *philo, t_action action)
{
	t_timeval	current;
	int32_t		ts;

	if (get_status(philo) != ALIVE_STATUS)
		return ;
	sem_wait(philo->print_sem);
	gettimeofday(&current, NULL);
	ts = delta_time(*philo->_start_sim, current);
	if (action == EAT)
		printf("%s % 6d -- phil %u is eating\n" RESET, GRN, ts, philo->id);
	else if (action == SLEEP)
		printf("%s % 6d -- phil %u is sleeping\n" RESET, MAG, ts, philo->id);
	else if (action == THINK)
		printf("%s % 6d -- phil %u is thinking\n" RESET, YEL, ts, philo->id);
	else if (action == FORK)
		printf("%s % 6d -- phil %u took a fork\n" RESET, BLU, ts, philo->id);
	else if (action == DIE)
		printf("%s % 6d -- phil %u died\n" RESET, RED, ts, philo->id);
	if (action != DIE)
		sem_post(philo->print_sem);
}
