/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by pessoa-b          #+#    #+#             */
/*   Updated: 2026/01/21 20:21:33 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	builtin_echo(char **args)
{
	int	i;
	int	n_flag;
		
	n_flag = 0;
	i = 1;

	if (ft_strncmp(args[0], "echo", 5) != 0)
	{
		handle_error()
		return (1);
	}
	while (args[i] &&  ft_strncmp(args[i], "-n", 2) == 0)
	{
		n_flag = 1;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			write(1, " ", 1);
		if (n_flag == 0)
		{
			write(1, "\n", 1);
		}
		i++;
	}
	return (0);
}
