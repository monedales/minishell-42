/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by pessoa-b          #+#    #+#             */
/*   Updated: 2026/03/04 21:36:27 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Built-in implementation of the echo command
 *
 * Prints all arguments separated by a single space to stdout.
 * Supports consecutive -n flags at the start of the argument list,
 * which suppress the trailing newline.
 *
 * Examples:
 *   echo hello world     -> "hello world\n"
 *   echo -n hello        -> "hello"  (no newline)
 *   echo -n -n hello     -> "hello"  (multiple -n still suppresses newline)
 *   echo                 -> "\n"     (no args, just newline)
 *
 * @param args Array of arguments (args[0] = "echo", args[1..n] = words)
 * @return Always returns 0 (success)
 */
int	builtin_echo(char **args)
{
	int	i;
	int	n_flag;

	n_flag = 0;
	i = 1;
	while (args[i] && ft_strncmp(args[i], "-n", 3) == 0)
	{
		n_flag = 1;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!n_flag)
		write(1, "\n", 1);
	return (0);
}
