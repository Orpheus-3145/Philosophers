/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   threads.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: fra <fra@student.42.fr>                      +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/23 15:46:35 by fra           #+#    #+#                 */
/*   Updated: 2023/04/30 18:58:38 by faru          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	begin_simulation(t_deposit *depo)
{
	pthread_t	check_thread;
	t_philo		*phil;
	uint32_t	i;

	i = 0;
	while (i < depo->n_philos)
	{
		phil = depo->philos + i++;
		set_last_time_eat(phil);
		if (pthread_create(&phil->phil_thread, NULL, &eat_sleep_repeat, phil))
			kill_program(depo, "error in philos initialization", true);
	}
	if (pthread_create(&check_thread, NULL, &monitoring, depo))
		kill_program(depo, "error in checker initialization", true);
	if (pthread_join(check_thread, NULL))
		kill_program(depo, "error while joining monitoring thread", true);
	i = 0;
	while (i < depo->n_philos)
	{
		phil = depo->philos + i++;
		if (pthread_join(phil->phil_thread, NULL))
			kill_program(depo, "error while joining philos threads", true);
	}
}

void	*eat_sleep_repeat(void *param)
{
	t_philo	*self;

	self = (t_philo *) param;
	if (self->id % 2 == 0)
		ft_usleep(self, 10);
	set_last_time_eat(self);
	while (check_status(self) == ALIVE)
	{
		pthread_mutex_lock(self->l_fork);
		print_message(self, FORK);
		pthread_mutex_lock(&self->r_fork);
		print_message(self, FORK);
		set_last_time_eat(self);
		print_message(self, EAT);
		ft_usleep(self, self->t_eat);
		pthread_mutex_unlock(&self->r_fork);
		pthread_mutex_unlock(self->l_fork);
		if (self->meals && ! --self->meals)
			set_status(self, FULLY_ATE);
		print_message(self, SLEEP);
		ft_usleep(self, self->t_sleep);
		print_message(self, THINK);
	}
	return (NULL);
}

void	*monitoring(void *param)
{
	t_deposit	*depo;
	uint32_t	n_philo_fully_ate;
	uint32_t	i;

	depo = (t_deposit *) param;
	i = 0;
	n_philo_fully_ate = 0;
	while (true)
	{
		n_philo_fully_ate += check_status(depo->philos + i) == FULLY_ATE;
		if (n_philo_fully_ate == depo->n_philos)
		{
			depo->philos_fully_ate = true;
			return (NULL);
		}
		else if (is_dead(depo->philos + i))
			break ;
		i = (i + 1) % depo->n_philos;
	}
	i = 0;
	while (i < depo->n_philos)
		set_status(depo->philos + i++, DEAD);
	return (NULL);
}

bool	is_dead(t_philo *phil)
{
	t_timeval	current;
	bool		status;

	gettimeofday(&current, NULL);
	pthread_mutex_lock(&phil->time_mutex);
	if (delta_time(phil->last_time_eat, current) >= (int32_t) phil->t_death)
		status = true;
	else
		status = false;
	pthread_mutex_unlock(&phil->time_mutex);
	if (status)
		print_message(phil, DIE);
	return (status);
}

void	print_message(t_philo *philo, t_action action)
{
	t_timeval	current;
	uint32_t	ts;

	gettimeofday(&current, NULL);
	ts = delta_time(*(philo->_start_sim), current);
	if (check_status(philo) != ALIVE)
		return ;
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
}
