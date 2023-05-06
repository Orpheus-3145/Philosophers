/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semaphores.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fra <fra@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 19:01:09 by faru              #+#    #+#             */
/*   Updated: 2023/05/06 19:09:34 by fra              ###   ########.fr       */
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
