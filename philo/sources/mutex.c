/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mutex.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: faru <faru@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/30 15:12:04 by faru          #+#    #+#                 */
/*   Updated: 2023/04/30 16:50:50 by faru          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_status	check_status(t_philo *phil)
{
	t_status	status;

	pthread_mutex_lock(&phil->status_mutex);
	status = phil->status;
	pthread_mutex_unlock(&phil->status_mutex);
	return (status);
}

void	set_status(t_philo *phil, t_status value)
{
	pthread_mutex_lock(&phil->status_mutex);
	phil->status = value;
	pthread_mutex_unlock(&phil->status_mutex);
}

void	set_last_time_eat(t_philo *phil)
{
	pthread_mutex_lock(&phil->time_mutex);
	gettimeofday(&phil->last_time_eat, false);
	pthread_mutex_unlock(&phil->time_mutex);
}
