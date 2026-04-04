/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipeline.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by pessoa-b          #+#    #+#             */
/*   Updated: 2026/04/04 15:00:00 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Updates last_status and prints message based on signal received.
 *
 * Called when the last pipeline child is terminated by a signal.
 * Prints a newline for SIGINT (Ctrl+C) or "Quit (core dumped)" for SIGQUIT.
 *
 * @param status      Raw waitpid status from the child
 * @param last_status Pointer to the exit status to update
 */
void	handle_signal_status(int status, int *last_status)
{
	*last_status = 128 + WTERMSIG(status);
	if (WTERMSIG(status) == SIGINT)
		write(STDERR_FILENO, "\n", 1);
	else if (WTERMSIG(status) == SIGQUIT)
		write(STDERR_FILENO, "Quit (core dumped)\n", 19);
}

/**
 * @brief Forks all child processes in the pipeline and connects pipes.
 *
 * Iterates over the command list, creating a pipe for each non-last command,
 * forking a child, and passing pipe fds to the next iteration via prev_fd.
 * The parent closes its copies of used fds after each fork.
 *
 * @param cmd_list Linked list of commands to fork
 * @param mini     Main shell structure
 * @return 0 on success, 1 on pipe or fork error
 */
int	fork_pipeline(t_cmd *cmd_list, t_mini *mini)
{
	t_cmd	*cmd;
	int		prev_fd;
	int		pipefd[2];

	cmd = cmd_list;
	prev_fd = -1;
	while (cmd)
	{
		if (cmd->next && pipe(pipefd) == -1)
			return (1);
		cmd->pid = fork();
		if (cmd->pid == -1)
			return (1);
		if (cmd->pid == 0)
			child_process(cmd, prev_fd, pipefd, mini);
		if (prev_fd != -1)
			close(prev_fd);
		if (cmd->next)
		{
			close(pipefd[1]);
			prev_fd = pipefd[0];
		}
		cmd = cmd->next;
	}
	return (0);
}

/**
 * @brief Waits for all child processes and returns the last exit status.
 *
 * Iterates the command list collecting each child's exit status via waitpid.
 * Only the last command's status affects the return value, matching bash.
 * Handles signal termination via handle_signal_status, then restores
 * prompt-mode signal handlers.
 *
 * @param cmd_list Head of the command list (each node has a pid)
 * @param mini     Main shell structure
 * @return Exit status of the last command in the pipeline
 */
static int	wait_all(t_cmd *cmd_list, t_mini *mini)
{
	int		status;
	int		last_status;
	t_cmd	*cmd;

	last_status = 0;
	cmd = cmd_list;
	while (cmd)
	{
		if (cmd->pid > 0)
		{
			waitpid(cmd->pid, &status, 0);
			if (!cmd->next)
			{
				if (WIFEXITED(status))
					last_status = WEXITSTATUS(status);
				else if (WIFSIGNALED(status))
					handle_signal_status(status, &last_status);
			}
		}
		cmd = cmd->next;
	}
	mini->last_exit_status = last_status;
	setup_signals();
	return (last_status);
}

/**
 * @brief Executes a pipeline of commands connected by pipes.
 *
 * Delegates forking to fork_pipeline, then ignores signals in the parent
 * during execution (setup_exec_signals) and waits for all children via
 * wait_all, which also restores prompt-mode signal handlers.
 *
 * @param cmd_list Linked list of commands connected by pipes
 * @param mini     Main shell structure
 * @return Exit status of the last command
 */
int	execute_pipeline(t_cmd *cmd_list, t_mini *mini)
{
	if (fork_pipeline(cmd_list, mini) == 1)
		return (1);
	setup_exec_signals();
	return (wait_all(cmd_list, mini));
}
