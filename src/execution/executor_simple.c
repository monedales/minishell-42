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
/*
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
*/
char	*find_cmd_path(char *cmd_name, t_mini *mini)
{
	char	*path_value;
	char	**dir;
	char	*temp;
	char	*full_path;
	t_env	*env_list;
	int		i;

	env_list = mini->env;
    if (ft_strchr(cmd_name, '/'))
    	return (ft_strdup(cmd_name));
	while (env_list)
	{			
		if (ft_strncmp(env_list->key, "PATH", 4) == 0 
		&& env_list->key[4] == '\0')
		{
			path_value = env_list->value;
			break ;
		}
		env_list = env_list->next;
	}
	if (!path_value)
		return (NULL);
	temp = ft_strdup(path_value);
	dir = ft_split(path_value, ':');
	if (!dir)
    	return (NULL);
	i = 0;
	while (dir[i])
	{
		temp = ft_strjoin(dir[i], "/");
		full_path = ft_strjoin(temp, cmd_name);
		free(temp);
		if (access(full_path, X_OK) == 0)
		{
			free_array(dir);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	return (NULL);
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

int	execute_simple_cmd(t_cmd *cmd, t_mini *mini)
{

	// aqui e pra lidar com os cmd og do linux (ls, cat e tal)
	pid_t	pid;
	char	*cmd_path;
	char	**env_array;

	cmd_path = find_cmd_path(cmd->args[0], mini);
	if (!cmd_path)
		return (1);
		//return (print_command_error(cmd->args[0]));
	env_array = join_key_value(mini->env);
	pid = fork();
	if (pid == -1)
	{
		perror("error fork");
		free(cmd_path);
		free_array(env_array);
		return (1);
	}
	if (pid == 0)
	{
		if (execve(cmd_path, cmd->args, env_array) == -1)
		{
			ft_putstr_fd("error execve", 2);
			free(cmd_path);
			free_array(env_array);
			exit(126);
		}
	}
	else {
		wait(NULL);
	} 
	return (0);
}
/*
int	main(int ac, char **av)
{
	t_cmd *cmd;
	t_mini *mini;
	
	char	*args[] = {"ls", "-la"}
	//ls
	execute_simple_cmd(mini->cmd_list, mini);
	return 0;
}

typedef struct s_mini
{
	t_env	*env;				// lista ligada de variáveis de ambiente
	t_cmd	*cmd_list;			// lista de comandos (separados por pipe)
	int		last_exit_status;	// status do último comando ($?)
	int		running;			// flag para manter o loop ativo
}	t_mini;
	

typedef struct s_cmd
{
	char			**args;		// comando + argumentos [cmd, arg1, arg2, NULL]
	t_redir			*redirs;	// lista de redirecionamentos
	struct s_cmd	*next;		// próximo comando (após pipe)
}	t_cmd;
	

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*prev;
	struct s_env	*next;
}	t_env;


typedef struct s_redir
{
	t_token_type	type;		// tipo de redirecionamento
	char			*file;		// nome do arquivo
	struct s_redir	*next;
}	t_redir;
*/