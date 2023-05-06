/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fra <fra@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 18:25:25 by fra               #+#    #+#             */
/*   Updated: 2023/05/06 19:14:04 by fra              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

bool	create_depo(int32_t argc, char **argv, t_deposit *depo)
{
	t_philo		*philo;
	uint32_t	i;

	if (! depo)
		return (false);
	depo->n_philos = ft_atoui(argv[0]);
	if (! new_sem("/forks_semaphore", depo->forks_sem, depo->n_philos))
		return (false);
	if (! new_sem("/print_semaphore", depo->print_sem, 1))
		return (false);
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
		i++;
	}
	return (true);
}

bool	create_philo(t_philo *phil, int32_t argc, char **argv, uint32_t id)
{
	phil->status = ALIVE_STATUS;
	phil->id = id;
	phil->t_death = ft_atoui(argv[1]);
	phil->t_eat = ft_atoui(argv[2]);
	phil->t_sleep = ft_atoui(argv[3]);
	if (argc == 5)
		phil->meals = ft_atoui(argv[4]);
	else
		phil->meals = 0;
	phil->status_sem_name = create_name_sem(id, "/status_semaphore_");
	if (! phil->status_sem_name)
		return (false);
	if (! new_sem(phil->status_sem_name, phil->status_sem, 1))
		return (false);
	phil->time_sem_name = create_name_sem(id, "/time_semaphore_");
	if (! phil->time_sem_name)
		return (false);
	if (! new_sem(phil->time_sem_name, phil->time_sem, 1))
		return (false);
	else
		return (true);
}

char	*create_name_sem(uint32_t id, char	*fixed)
{
	char		*final;
	uint32_t	size;
	uint32_t	i;
	uint32_t	j;

	size = 3;
	while (fixed[size++ - 3])
		;
	final = malloc(size * sizeof(char));
	if (final)
	{
		final[--size] = '\0';
		j = 3;
		while (j--)
		{
			final[--size] = id % 10 + '0';
			id /= 10;
		}
		i = 0;
		while (i++ < size)
			final[i - 1] = fixed[i - 1];
	}
	return (final);
}

bool	new_sem(char *sem_name, sem_t *semaphore, int32_t amount)
{
	sem_unlink(sem_name);
	semaphore = sem_open(sem_name, O_CREAT | O_EXCL, 0666, amount);
	return (semaphore != SEM_FAILED);
}

void	free_depo(t_deposit *depo)
{
	uint32_t	i;

	if (! depo)
		return ;
	if (depo->philos)
	{
		i = 0;
		while (i < depo->n_philos)
		{
			sem_close(depo->philos[i].status_sem);
			sem_unlink(depo->philos[i].status_sem_name);
			free(depo->philos[i].status_sem_name);
			sem_close(depo->philos[i].time_sem);
			sem_unlink(depo->philos[i].time_sem_name);
			free(depo->philos[i].time_sem_name);
			i++;
		}
		free(depo->philos);
	}
	sem_close(depo->forks_sem);
	sem_unlink("/forks_semaphore");
	sem_close(depo->print_sem);
	sem_unlink("/print_semaphore");
	free(depo);
}
