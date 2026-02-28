/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by pessoa-b          #+#    #+#             */
/*   Updated: 2026/01/21 20:21:33 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Validates that a string is a valid shell identifier.
 * 
 * A valid identifier starts with a letter or '_', followed by
 * letters, digits, or '_' only. Unlike export, unset never
 * accepts '=' in the argument.
 * 
 * @param str The argument string
 * @return TRUE if valid, FALSE otherwise
 */
static int	is_valid_identifier_unset(char *args)
{
	int	i;

	if (!args || args[0] == '\0')
		return (0);
	if (!ft_isalpha(args[0]) && args[0] != '_')
		return (0);
	i = 1;
	while (args[i])
	{
		if (!ft_isalnum(args[i]) && args[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

/**
 * @brief Builtin implementation of the unset command.
 * 
 * Removes each given variable from the shell environment.
 * With no arguments, does nothing and returns SUCCESS.
 * If a variable is not found, it is silently ignored (not an error).
 * If any argument is an invalid identifier, prints an error and
 * sets the return value to 1, but continues processing remaining args.
 * 
 * @param args NULL-terminated argument array (args[0] == "unset")
 * @param mini The main shell structure
 * @return SUCCESS (0) if all args are valid identifiers, 1 otherwise
 */
int	builtin_unset(char **args, t_mini *mini)
{
	int	i;
	int	ret;

	ret = 0;
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier_unset(args[i]))
		{
			handle_error(ERR_NOT_VALID_ID, "unset", args[i]);
			ret = 1;
		}
		else
			unset_env_value(&mini->env, args[i]);
		i++;
	}
	return (ret);
}
