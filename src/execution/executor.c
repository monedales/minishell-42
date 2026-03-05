/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by pessoa-b          #+#    #+#             */
/*   Updated: 2026/03/04 21:48:46 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Main dispatcher for command list execution.
 *
 * Determines how to execute the command list based on its content:
 * - If args[0] is a builtin: executes it directly in the parent process
 *   via execute_builtin, so changes to the shell state (env, cwd) persist.
 * - If it is a single external command: delegates to execute_simple_cmd.
 * - Pipeline support (multiple commands separated by pipes).
 *
 * @param cmd_list Linked list of commands to execute
 * @param mini     Pointer to the main shell structure
 * @return Exit status of the last executed command (currently always 0)
 */
int	execute_cmd_list(t_cmd *cmd_list, t_mini *mini)
{
	char	*command;

	command = cmd_list->args[0];
	if (is_builtin(command) == 1)
		execute_builtin(cmd_list, mini);
	else if (command && cmd_list->next == NULL)
		execute_simple_cmd(cmd_list, mini);
	/*
	else
		execute_pipeline(cmd_list, mini);
	*/
	return (0);
}
