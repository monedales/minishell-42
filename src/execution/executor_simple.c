/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_simple.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <maria-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by pessoa-b          #+#    #+#             */
/*   Updated: 2026/03/12 21:04:20 by maria-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Waits for the child process and returns its exit status.
 * 
 * Uses waitpid to collect the child and extracts the exit code
 * via WEXITSTATUS, updating mini->last_exit_status.
 * 
 * @param pid  PID of the child process
 * @param mini Main shell structure
 * @return Exit status of the child
 */
int	wait_child(pid_t pid, t_mini *mini)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		mini->last_exit_status = WEXITSTATUS(status);
		return (mini->last_exit_status);
	}
	if (WIFSIGNALED(status))
	{
		mini->last_exit_status = 128 + WTERMSIG(status);
		return (mini->last_exit_status);
	}
	return (1);
}

/**
 * @brief Resolves the command path and executes it with execve.
 * 
 * Called in the child process. Sets up redirections first,
 * then finds the command path and calls execve.
 * Exits with appropriate code on any error.
 * 
 * @param cmd  Command node
 * @param mini Main shell structure
 */
void	exec_child(t_cmd *cmd, t_mini *mini)
{
	char	*cmd_path;
	char	**env_array;

	if (setup_redirections(cmd->redirs) == ERROR)
		exit(1);
	cmd_path = find_command_path(cmd->args[0], mini->env);
	if (!cmd_path)
	{
		handle_error(ERR_CMD_NOT_FOUND, cmd->args[0], NULL);
		exit(127);
	}
	env_array = env_to_array(mini->env);
	if (!env_array)
		exit(1);
	execve(cmd_path, cmd->args, env_array);
	perror(cmd->args[0]);
	free(cmd_path);
	free_array(env_array);
	exit(126);
}

/**
 * @brief Executes a single command without pipes.
 * 
 * If the command is a builtin, runs it directly in the parent process.
 * Otherwise forks, runs exec_child in the child, and waits for it.
 * 
 * @param cmd  Command node to execute
 * @param mini Main shell structure
 * @return Exit status of the command
 */
int	execute_simple_cmd(t_cmd *cmd, t_mini *mini)
{
	pid_t	pid;

	if (is_builtin(cmd->args[0]) == 1)
		return (execute_builtin(cmd, mini));
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		return (1);
	}
	if (pid == 0)
		exec_child(cmd, mini);
	return (wait_child(pid, mini));
}
