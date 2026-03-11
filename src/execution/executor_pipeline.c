/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipeline.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by pessoa-b          #+#    #+#             */
/*   Updated: 2026/01/21 20:21:33 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Sets up stdin/stdout for a child process in the pipeline.
 *
 * Connects stdin to the previous pipe's read end (if any),
 * and stdout to the current pipe's write end (if not the last command).
 * Closes all fds that the child no longer needs after dup2.
 *
 * @param prev_fd Read end of the previous pipe (-1 if first command)
 * @param pipefd  Current pipe fds (only used if not last command)
 * @param is_last TRUE if this is the last command in the pipeline
 */
static void	setup_child_fds(int prev_fd, int *pipefd, int is_last)
{
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (!is_last)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
	}
}

/**
 * @brief Executes a command inside a child process.
 *
 * If the command is a builtin, runs it and exits with its return value.
 * Otherwise, resolves the path and calls execve.
 * Never returns on success.
 *
 * @param cmd  Command node to execute
 * @param mini Main shell structure
 */
static void	exec_pipeline_cmd(t_cmd *cmd, t_mini *mini)
{
	char	*path;
	char	**env_array;

	if (is_builtin(cmd->args[0]))
		exit(execute_builtin(cmd, mini));
	path = find_command_path(cmd->args[0], mini->env);
	if (!path)
	{
		handle_error(ERR_CMD_NOT_FOUND, cmd->args[0], NULL);
		exit(127);
	}
	env_array = env_to_array(mini->env);
	if (!env_array)
		exit(1);
	execve(path, cmd->args, env_array);
	perror(cmd->args[0]);
	free(path);
	free_array(env_array);
	exit(126);
}

/**
 * @brief Entry point for a child process in the pipeline.
 *
 * Sets up pipe fds, applies explicit redirections (which override
 * pipe fds if present), then executes the command.
 *
 * @param cmd     Command to execute
 * @param prev_fd Read end of the previous pipe (-1 if first command)
 * @param pipefd  Current pipe fds
 * @param is_last TRUE if this is the last command in the pipeline
 * @param mini    Main shell structure
 */
static void	child_process(t_cmd *cmd, int prev_fd, int *pipefd,
				int is_last, t_mini *mini)
{
	setup_child_fds(prev_fd, pipefd, is_last);
	if (setup_redirections(cmd->redirs) == ERROR)
		exit(1);
	exec_pipeline_cmd(cmd, mini);
}

/**
 * @brief Waits for all child processes and returns the last exit status.
 *
 * Iterates the command list using the stored PIDs.
 * Only the status of the last command affects the return value,
 * matching bash behavior.
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
					last_status = 128 + WTERMSIG(status);
			}
		}
		cmd = cmd->next;
	}
	mini->last_exit_status = last_status;
	return (last_status);
}

/**
 * @brief Executes a pipeline of commands connected by pipes.
 *
 * For each command:
 *   - Creates a pipe (except for the last command)
 *   - Forks a child process
 *   - The child sets up its fds and executes
 *   - The parent closes used fds and passes the read end to the next cmd
 * After all forks, waits for all children and returns last exit status.
 *
 * @param cmd_list Linked list of commands connected by pipes
 * @param mini     Main shell structure
 * @return Exit status of the last command
 */
int	execute_pipeline(t_cmd *cmd_list, t_mini *mini)
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
			child_process(cmd, prev_fd, pipefd, !cmd->next, mini);
		if (prev_fd != -1)
			close(prev_fd);
		if (cmd->next)
		{
			close(pipefd[1]);
			prev_fd = pipefd[0];
		}
		cmd = cmd->next;
	}
	return (wait_all(cmd_list, mini));
}
