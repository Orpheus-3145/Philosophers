/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fra <fra@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 14:12:51 by fra               #+#    #+#             */
/*   Updated: 2023/05/06 19:24:50 by fra              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

bool	check_input(int32_t argc, char **argv)
{
	int32_t	i;
	int32_t	j;

	if (argc != 5 && argc != 6)
		return (false);
	i = 1;
	while (i < argc)
	{
		j = 0;
		if (argv[i][j] == '+')
			j++;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (false);
			j++;
		}
		if (! ft_atoui(argv[i]))
		{
			if (i == 1 || i == 5 || (i > 1 && argv[i][0] != '0'))
				return (false);
		}
		i++;
	}
	return (ft_atoui(argv[1]) < 126);
}

void	kill_program(t_deposit *depo, char *message)
{
	uint32_t	len_mess;

	if (depo)
		free_depo(depo);
	if (message)
	{
		len_mess = 0;
		while(message[len_mess++]);
		write(2, message, len_mess);
		write(2, "\n", 1);
	}
	exit(EXIT_FAILURE);
}

uint32_t	ft_atoui(const char *str)
{
	uint32_t	nbr;

	nbr = 0;
	if (*str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
		nbr = (nbr * 10) + (*str++ - '0');
	return (nbr);
}

int32_t	delta_time(t_timeval t1, t_timeval t2)
{
	t_timeval	delta;
	int32_t		timestamp;

	while ((t2.tv_usec < t1.tv_usec) && (t2.tv_sec > t1.tv_sec))
	{
		t2.tv_usec += 1000000;
		t2.tv_sec -= 1;
	}
	delta.tv_sec = t2.tv_sec - t1.tv_sec;
	delta.tv_usec = t2.tv_usec - t1.tv_usec;
	timestamp = delta.tv_sec * 1000000 + delta.tv_usec;
	return (timestamp / 1000);
}

void	ft_msleep(t_philo *phil, uint32_t milli_secs)
{
	t_timeval	start;
	t_timeval	current;

	gettimeofday(&start, NULL);
	gettimeofday(&current, NULL);
	while (delta_time(start, current) < (int32_t) milli_secs)
	{
		if (get_status(phil) != ALIVE_STATUS)
			break ;
		usleep(50);
		gettimeofday(&current, NULL);
	}
}

