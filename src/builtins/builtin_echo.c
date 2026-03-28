/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by pessoa-b          #+#    #+#             */
/*   Updated: 2026/03/28 16:08:46 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Checks if an argument is a valid -n flag for echo.
 *
 * A valid flag is a '-' followed by one or more 'n' characters only.
 * Examples:
 *   "-n"        → TRUE
 *   "-nnn"      → TRUE
 *   "-nnnnnnn"  → TRUE
 *   "-nngn"     → FALSE (non-n character)
 *   "-"         → FALSE (no n after dash)
 *   "n"         → FALSE (no leading dash)
 *
 * @param arg The argument string to check.
 * @return TRUE if valid -n flag, FALSE otherwise.
 */
static int	is_n_flag(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-' || arg[1] == '\0')
		return (FALSE);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (FALSE);
		i++;
	}
	return (TRUE);
}

/**
 * @brief Built-in implementation of the echo command
 *
 * Prints all arguments separated by a single space to stdout.
 * Supports consecutive -n[n...] flags at the start of the argument list,
 * which suppress the trailing newline. Any number of 'n's is accepted
 * as long as the argument contains only '-' followed by 'n's.
 *
 * Examples:
 *   echo hello world     -> "hello world\n"
 *   echo -n hello        -> "hello"   (no newline)
 *   echo -nnn hello      -> "hello"   (no newline)
 *   echo -n -nnn hello   -> "hello"   (multiple flags, no newline)
 *   echo -nngn hello     -> "-nngn hello\n"  (invalid flag, printed as-is)
 *   echo                 -> "\n"
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
	while (args[i] && is_n_flag(args[i]))
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
