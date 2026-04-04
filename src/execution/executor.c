/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by pessoa-b          #+#    #+#             */
/*   Updated: 2026/04/04 15:00:00 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Counts the number of commands in the pipeline list.
 *
 * @param cmd_list Head of the command list
 * @return Number of commands
 */
static int	count_cmds(t_cmd *cmd_list)
{
	int		count;
	t_cmd	*current;

	count = 0;
	current = cmd_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

/**
 * @brief Executes a builtin in the parent process with redirections.
 *
 * Saves original stdin/stdout, applies redirections, runs the builtin,
 * then restores the original file descriptors.
 * This ensures that redirections like "echo hi > file" work for builtins
 * without permanently affecting the shell's own stdin/stdout.
 *
 * @param cmd  Command node
 * @param mini Main shell structure
 * @return Exit status of the builtin, or 1 on redirection error
 */
static int	exec_builtin_parent(t_cmd *cmd, t_mini *mini)
{
	int	saved_in;
	int	saved_out;
	int	ret;

	saved_in = dup(STDIN_FILENO);
	saved_out = dup(STDOUT_FILENO);
	if (setup_redirections(cmd->redirs) == ERROR)
	{
		restore_fds(saved_in, saved_out);
		return (1);
	}
	ret = execute_builtin(cmd, mini);
	restore_fds(saved_in, saved_out);
	mini->last_exit_status = ret;
	return (ret);
}

int	execute_cmd_list(t_cmd *cmd_list, t_mini *mini)
{
	if (!cmd_list || !cmd_list->args || !cmd_list->args[0])
		return (1);
	if (collect_all_heredocs(cmd_list, mini) == ERROR)
	{
		if (g_signal == 130)
		{
			mini->last_exit_status = 130;
			g_signal = 0;
		}
		return (mini->last_exit_status);
	}
	if (count_cmds(cmd_list) == 1)
	{
		if (is_builtin(cmd_list->args[0]))
			return (exec_builtin_parent(cmd_list, mini));
		return (execute_simple_cmd(cmd_list, mini));
	}
	return (execute_pipeline(cmd_list, mini));
}
