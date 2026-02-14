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

#include "../minishell.h"

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

	char	*find_cmd_path(char *args, t_mini *mini)
	{
		char	*path_value;
		char	*path_copy;
		t_env	*env_list;
		char	**dir;

		env_list = mini->env;
		while (env_list->next)
		{
			if (ft_strncmp(env_list->key, "PATH", 5) == 0)
			{
				path_value = env_list->value;
				break ;
			}
			env_list = env_list->next;
		}
		if (!path_value)
			return (NULL);
		path_copy = ft_strdup(path_value);
		dir = ft_split(path_value, ":");
			while (*dir + 1)
			{
				// aqui enquanto tiver diretorios /bin/usr
				t_pid id = fork();
				if (id == -1)// error
					perror("pid error");
				if (id == 0) // child
				{
					execve();
					// precisa transformar o struct pra char *[] "USER=admin"? "key" "value" 
					// char **env_cpy = join_key_value(env_list->key, env_list->value)
					/* typedef struct s_env
						{
							char			*key;
							char			*value;
							struct s_env	*prev;
							struct s_env	*next;
						}	t_env;*/
					char **temp_env;
					
					execve(const char *filename, char *const argv[], char *const envp[])
				}
				else 		 // father
			}
			// montar a tentativa (full_path) para o access exemplo "bin/usr/ls" ou "bin/usr/cat"
			// aqui vai usar o access
			// retornar o full_path
		return (NULL);
	}


int	size_list(t_env *env)
{
	t_env	*temp;
	int		count;

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
	int		size;
	int		i;
	t_env	*temp;

	size = size_list(env);
	temp = env;
	i = 0;
	while (i < size && temp)
	{
		temp_key = ft_strjoin(temp->key, "=");
		key_value[i] = ft_strjoin(temp_key, env->value);
		temp = temp->next;
	}
	key_value[i] = NULL;
	return (key_value);
	
	// strjoin?, 
}

int	execute_simple_cmd(t_cmd *cmd, t_mini *mini)
{

	// aqui e pra lidar com os cmd og do linux (ls, cat e tal)
	// precisa procurar por PATH na env
	char	*cmd_path = find_cmd_path(cmd->args[0], mini);

	t_pid	pid;
	char	**args;

	args = cmd->args;
	pid = fork();
	if (pid == -1)
		ft_putstr_fd("error pid") // TODO
	if (pid == 0)
	{
		if (execve(args[0], args) == -1)
		{
			ft_putstr_fd("error execve");
		}
		exit(1);
	}
	else {
		wait(NULL);
		ft_putstr_fd("comando executado");
	}
	// precisa procurar dentro do path o caminho do cmd "/bin/usr/ls"
	// env cospe 
	return (0);
}
