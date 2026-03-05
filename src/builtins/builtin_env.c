/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by pessoa-b          #+#    #+#             */
/*   Updated: 2026/03/04 21:36:56 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Built-in implementation of the env command
 *
 * Prints all environment variables to stdout in "KEY=VALUE" format,
 * one per line. Delegates the printing to print_env() from env_utils.c.
 * Returns 1 if the environment list is empty or NULL.
 *
 * @param mini Pointer to the main shell structure (env list)
 * @return 0 on success, 1 if the environment list is empty
 */
int	builtin_env(t_mini *mini)
{
	t_env	*env;

	env = mini->env;
	if (!env)
		return (1);
	print_env(env);
	return (0);
}
