/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_simple.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by pessoa-b          #+#    #+#             */
/*   Updated: 2026/01/21 20:21:33 by mona             ###   ########.fr       */
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
 * @brief Executa um comando simples (sem pipes)
 * 
 * TODO (Pessoa B): Implementar
 * - Setup dos redirecionamentos
 * - Fork
 * - No filho: execve
 * - No pai: waitpid e capturar exit status
 * 
 * @param cmd Comando a executar
 * @param mini Estrutura principal
 * @return Exit status do comando
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
	handle_error(ERR_PERMISSION, cmd->args[0], 0);
	free(cmd_path);
	free_split(env_array);
	exit(126);
}

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
