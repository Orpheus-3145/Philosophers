/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fra <fra@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 18:25:25 by fra               #+#    #+#             */
/*   Updated: 2023/05/06 19:22:32 by fra              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

bool	create_depo(int32_t argc, char **argv, t_deposit *depo)
{
	t_philo		*philo;
	uint32_t	i;	

	depo->n_philos = ft_atoui(argv[0]);
	depo->philos_fully_ate = false;
	depo->philos = malloc(depo->n_philos * sizeof(t_philo));
	if (! depo->philos)
		return (false);
	i = 0;
	while (i < depo->n_philos)
	{
		philo = depo->philos + i;
		if (! create_philo(philo, argc, argv, i + 1))
			return (false);
		philo->_start_sim = &depo->_start_sim;
		if (i++)
			philo->l_fork = &(philo - 1)->r_fork;
	}
	depo->philos[0].l_fork = &depo->philos[i - 1].r_fork;
	return (true);
}

bool	create_philo(t_philo *phil, int32_t argc, char **argv, uint32_t id)
{
	phil->status = ALIVE;
	phil->id = id;
	phil->t_death = ft_atoui(argv[1]);
	phil->t_eat = ft_atoui(argv[2]);
	phil->t_sleep = ft_atoui(argv[3]);
	if (argc == 5)
		phil->meals = ft_atoui(argv[4]);
	else
		phil->meals = 0;
	if (pthread_mutex_init(&phil->time_mutex, NULL))
		return (false);
	else if (pthread_mutex_init(&phil->status_mutex, NULL))
		return (false);
	else if (pthread_mutex_init(&phil->r_fork, NULL))
		return (false);
	else
		return (true);
}

void	free_depo(t_deposit *depo)
{
	uint32_t	i;

	i = 0;
	if (! depo)
		return ;
	if (depo->philos)
	{
		while (i < depo->n_philos)
		{
			pthread_mutex_destroy(&depo->philos[i].time_mutex);
			pthread_mutex_destroy(&depo->philos[i].status_mutex);
			pthread_mutex_destroy(&depo->philos[i].r_fork);
			i++;
		}
		free(depo->philos);
	}
	free(depo);
}
