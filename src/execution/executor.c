/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <maria-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by pessoa-b          #+#    #+#             */
/*   Updated: 2026/03/12 21:04:20 by maria-ol         ###   ########.fr       */
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
 
/**
 * @brief Main dispatcher for command list execution.
 * 
 * Decides how to execute based on number of commands:
 *   - 1 command, builtin -> exec_builtin_parent (runs in parent)
 *   - 1 command, external -> execute_simple_cmd (fork + execve)
 *   - 2+ commands -> execute_pipeline (pipes + forks)
 * 
 * @param cmd_list List of commands to execute
 * @param mini     Main shell structure
 * @return Exit status of the last command
 */
/**
 * @brief Collects all heredoc inputs for a command list in the parent process.
 *
 * Walks every command's redir list and pre-fills each TKN_REDIR_HEREDOC node
 * with a pipe read fd via collect_heredoc(). This must run before any fork so
 * that readline never executes inside a child process — preventing the
 * 'still reachable' leak that occurs when Ctrl+C kills a child mid-readline.
 *
 * If Ctrl+C is received during collection, closes already-collected fds and
 * returns ERROR so execute_cmd_list can propagate the 130 exit status.
 *
 * @param cmd_list Linked list of commands to process.
 * @param mini     Main shell structure (used to update last_exit_status).
 * @return SUCCESS if all heredocs collected, ERROR on signal or pipe failure.
 */
int	collect_all_heredocs(t_cmd *cmd_list, t_mini *mini)
{
	t_cmd	*cmd;
	t_redir	*redir;
 
	(void)mini;
	cmd = cmd_list;
	while (cmd)
	{
		redir = cmd->redirs;
		while (redir)
		{
			if (redir->type == TKN_REDIR_HEREDOC)
			{
				redir->fd = collect_heredoc(redir->file);
				if (redir->fd == -1)
					return (ERROR);
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (SUCCESS);
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