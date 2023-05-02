/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: fra <fra@student.42.fr>                      +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/23 13:56:49 by fra           #+#    #+#                 */
/*   Updated: 2023/04/30 19:07:54 by faru          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void	kill_program(t_deposit *depo, char *message)
{
	if (depo)
		free_depo(depo);
	printf("\t-- %s --\n", message);
	exit(EXIT_FAILURE);
}

int	main(int argc, char **argv)
{
	t_deposit	*depo;

	if (check_input(argc, argv) == 0)
		kill_program(NULL, "input error");
	depo = malloc(sizeof(t_deposit));
	if (! create_depo(--argc, ++argv, depo))
		kill_program(depo, "program killed, failed to initilize");
	gettimeofday(&depo->_start_sim, NULL);
	if (depo->n_philos == 1)
	{
		print_message(depo->philos, FORK);
		ft_usleep(depo->philos, depo->philos->t_death);
		print_message(depo->philos, DIE);
	}
	else if (depo->n_philos > 200)
		printf("Too many philosophers! The CPU is going to melt!\n");
	else if (depo->philos->t_death == 0)
		print_message(depo->philos, DIE);
	else
	{
		depo->_start_sim.tv_sec += 1;
		begin_simulation(depo);
	}
	free_depo(depo);
	return (EXIT_SUCCESS);
}
