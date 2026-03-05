/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_simple.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by pessoa-b          #+#    #+#             */
/*   Updated: 2026/03/04 21:26:31 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

 
void	free_array(char **array)
{
	int	i;

	if (!array)
		return;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

int	size_list(t_env *env)
{
	t_env	*temp;
	int		count;

	temp = env;
	count = 0;
	while (temp)
	{
		count++;
		temp = temp->next;
	}
	return (count);
}

void	exec_child(t_cmd *cmd, t_mini *mini)
{
	char	*cmd_path;
	char	**env_array;
	t_env	*env;

	env = mini->env;
	cmd_path = find_command_path(cmd->args[0], env);
	if (!cmd_path)
	{
		// handle error da mona
		exit(1);
	}
	env_array = env_to_array(mini->env);
	if (execve(cmd_path, cmd->args, env_array) == -1)
	{
		ft_putstr_fd("error execve", 2);
		free(cmd_path);
		free_array(env_array);
		exit(126);
	}
	exit(0);
}

int	execute_simple_cmd(t_cmd *cmd, t_mini *mini)
{
	pid_t	pid;

	if (is_builtin(cmd->args[0]) == 1)
		return (execute_builtin(cmd, mini));

	pid = fork();
	if (pid == -1)
	{
		perror("error fork");
		return (1);
	}
	if (pid == 0)
		exec_child(cmd, mini);
	else 
	{
		wait(NULL);
	} 
	return (1);
}
