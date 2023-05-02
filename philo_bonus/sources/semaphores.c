/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   semaphores.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: faru <faru@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/30 19:01:09 by faru          #+#    #+#                 */
/*   Updated: 2023/04/30 19:02:42 by faru          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

int32_t	get_status(t_philo *phil)
{
	int32_t	status;

	sem_wait(phil->status_sem);
	status = phil->status;
	sem_post(phil->status_sem);
	return (status);
}

void	set_status(t_philo *phil, int32_t value)
{
	sem_wait(phil->status_sem);
	phil->status = value;
	sem_post(phil->status_sem);
}

void	set_last_time_eat(t_philo *phil)
{
	sem_wait(phil->time_sem);
	gettimeofday(&phil->last_time_eat, NULL);
	sem_post(phil->time_sem);
}

void	open_sems(t_philo *phil)
{
	phil->forks_sem = sem_open("/forks_semaphore", 0);
	phil->print_sem = sem_open("/print_semaphore", 0);
	phil->status_sem = sem_open(phil->st_sem_name, 0);
	phil->time_sem = sem_open(phil->tm_sem_name, 0);
}

void	close_sems(t_philo *phil)
{
	sem_close(phil->forks_sem);
	sem_close(phil->print_sem);
	sem_close(phil->status_sem);
	sem_close(phil->time_sem);
}
