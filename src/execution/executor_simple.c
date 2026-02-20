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

// essa funçao tem que ir pro arquivo path_finder.c
// char	*find_cmd_path(char *cmd_name, t_mini *mini)
// {
// 	char	*path_value;
// 	char	**dir;
// 	char	*temp;
// 	char	*full_path;
// 	t_env	*env_list;
// 	int		i;

// 	env_list = mini->env;
//     if (ft_strchr(cmd_name, '/'))
//     	return (ft_strdup(cmd_name));
// 	while (env_list)
// 	{			
// 		if (ft_strncmp(env_list->key, "PATH", 4) == 0 
// 		&& env_list->key[4] == '\0')
// 		{
// 			path_value = env_list->value;
// 			break ;
// 		}
// 		env_list = env_list->next;
// 	}
// 	if (!path_value)
// 		return (NULL);
// 	temp = ft_strdup(path_value);
// 	dir = ft_split(path_value, ':');
// 	if (!dir)
//     	return (NULL);
// 	i = 0;
// 	while (dir[i])
// 	{
// 		temp = ft_strjoin(dir[i], "/");
// 		full_path = ft_strjoin(temp, cmd_name);
// 		free(temp);
// 		if (access(full_path, X_OK) == 0)
// 		{
// 			free_array(dir);
// 			return (full_path);
// 		}
// 		free(full_path);
// 		i++;
// 	}
// 	return (NULL);
// }


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

char	**join_key_value(t_env *env)
{
	char	**key_value;
	char	*temp_key;
	int		size;
	int		i;
	t_env	*temp;

	size = size_list(env);
	key_value = malloc(sizeof(char *) * (size + 1));
	if (!key_value)
		return (NULL);
	temp = env;
	i = 0;
	while (i < size && temp)
	{
		temp_key = ft_strjoin(temp->key, "=");
        if (!temp_key)
        {
            free_array(key_value);
            return (NULL);
        }
		key_value[i] = ft_strjoin(temp_key, temp->value);
        free(temp_key);
        if (!key_value[i])
		{
			free_array(key_value);
            return (NULL);
        }
		temp = temp->next;
        i++;
	}
	key_value[i] = NULL;
	return (key_value);
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
	env_array = join_key_value(mini->env);
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

	// cmd->args[0], ' '
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

// int main (int ac, char **av, char **envp)
// {
// 	t_cmd	*cmd;
// 	char	*args[] = {"echo ", "osasco", NULL};
// 	cmd->args = args;
// 	cmd->redirs = NULL;
// 	cmd->next = NULL;

// 	t_mini  *mini =  malloc(sizeof(t_mini));
// 	mini->env = init_env(envp);
//     mini->cmd_list = cmd;
//     mini->last_exit_status = 0;
// 	mini->running = 1;

// 	int	exit_statuss = execute_simple_cmd(cmd, mini);

// 	printf("%d\n", exit_statuss);
// 	return 0;	
// }