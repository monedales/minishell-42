/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_simple.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by pessoa-b          #+#    #+#             */
/*   Updated: 2026/03/26 20:36:59 by mona             ###   ########.fr       */
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
	setup_signals();
	if (WIFSIGNALED(status))
	{
		mini->last_exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT)
			write(STDERR_FILENO, "\n", 1);
		else if (WTERMSIG(status) == SIGQUIT)
			write(STDERR_FILENO, "Quit (core dumped)\n", 19);
		return (mini->last_exit_status);
	}
	if (WIFEXITED(status))
	{
		mini->last_exit_status = WEXITSTATUS(status);
		return (mini->last_exit_status);
	}
	return (1);
}

/**
 * @brief Frees inherited shell resources and exits in a child process.
 *
 * Called in forked children before exit() to release the copies of
 * env and cmd_list inherited from the parent via fork().
 * path and env_arr may be NULL — free/free_array handle it safely.
 *
 * @param mini    Main shell structure (owns cmd_list and env)
 * @param path    Resolved command path to free (or NULL)
 * @param env_arr env_array to free (or NULL)
 * @param code    Exit code
 */
static void	child_exit(t_mini *mini, char *path, char **env_arr, int code)
{
	free(path);
	free_array(env_arr);
	free_cmd_list(mini->cmd_list);
	free_env(mini->env);
	exit(code);
}

/**
 * @brief Resolves the command path and executes it with execve.
 *
 * Called in the child process. Sets up redirections first,
 * then finds the command path and calls execve.
 * All exit paths call child_exit to free inherited memory before exiting.
 *
 * @param cmd  Command node
 * @param mini Main shell structure
 */
void	exec_child(t_cmd *cmd, t_mini *mini)
{
	char	*cmd_path;
	char	**env_array;
 
	setup_child_signals();
	if (setup_redirections(cmd->redirs) == ERROR)
		child_exit(mini, NULL, NULL, 1);
	cmd_path = find_command_path(cmd->args[0], mini->env);
	if (!cmd_path)
	{
		handle_error(ERR_CMD_NOT_FOUND, cmd->args[0], NULL);
		child_exit(mini, NULL, NULL, 127);
	}
	env_array = env_to_array(mini->env);
	if (!env_array)
		child_exit(mini, cmd_path, NULL, 1);
	execve(cmd_path, cmd->args, env_array);
	perror(cmd->args[0]);
	child_exit(mini, cmd_path, env_array, 126);
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
	setup_exec_signals();
	return (wait_child(pid, mini));
}
