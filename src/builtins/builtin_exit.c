/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <maria-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by pessoa-b          #+#    #+#             */
/*   Updated: 2026/03/12 20:29:05 by maria-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Checks if a string represents a valid numeric argument for exit.
 * 
 * Accepts an optional leading '+' or '-', followed by one or more digits.
 * 
 * @param str The argument string
 * @return TRUE if numeric, FALSE otherwise
 */
int	is_numeric(const char *str)
{
	int	i;

	if (!str || str[0] == '\0')
		return (0);
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

/**
 * @brief Builtin implementation of the exit command.
 * 
 * Behavior:
 *   exit          → exits with last_exit_status
 *   exit <n>      → exits with n % 256 (numeric only)
 *   exit <str>    → prints "numeric argument required", exits with 2
 *   exit <n> <m>  → prints "too many arguments", returns 1 (does NOT exit)
 * 
 * Frees the environment before calling exit().
 * 
 * @param args NULL-terminated argument array (args[0] == "exit")
 * @param mini The main shell structure
 * @return 1 if too many arguments (no exit), otherwise does not return
 */
int	builtin_exit(char **args, t_mini *mini)
{
	int	code;

	if (!args[1])
	{
		mini->running = FALSE;
		return (mini->last_exit_status);
	}
	if (args[2])
	{
		handle_error(ERR_TOO_MANY_ARGS, "exit", NULL);
		return (1);
	}
	if (!is_numeric(args[1]))
	{
		handle_error(ERR_NUM_REQUIRED, "exit", args[1]);
		mini->running = FALSE;
		return (2);
	}
	code = ft_atoi(args[1]) % 256;
	if (code < 0)
		code += 256;
	mini->running = FALSE;
	return (code);
}
