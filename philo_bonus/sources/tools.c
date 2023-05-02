/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tools.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: fra <fra@student.42.fr>                      +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/23 14:12:51 by fra           #+#    #+#                 */
/*   Updated: 2023/04/30 19:08:02 by faru          ########   odam.nl         */
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

void	ft_usleep(t_philo *phil, uint32_t milli_secs)
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
