/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_finder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by pessoa-b          #+#    #+#             */
/*   Updated: 2026/01/21 20:21:33 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Searches for the executable in each directory of PATH.
 * 
 * Iterates over the split PATH dirs, builds the full path and
 * tests with access(). Frees the dirs array before returning.
 * 
 * @param dirs  NULL-terminated array of directories from PATH
 * @param cmd   Command name to look for
 * @return Full executable path (malloced), or NULL if not found
 */
char	*search_in_dirs(char **dirs, char *cmd)
{
	char	*tmp;
	char	*full_path;
	int		i;

	i = 0;
	while (dirs[i])
	{
		tmp = ft_strjoin(dirs[i], "/");
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(full_path, X_OK))
		{
			free_split(dirs);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_split(dirs);
	return (NULL);
}

/**
 * @brief Retrieves the PATH value from the environment list.
 * 
 * @param env Environment list
 * @return The value of PATH, or NULL if not found
 */
char	*get_path_value(t_env *env)
{
	while (env)
	{
		if (ft_strncmp(env->key, "PATH", 5) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

/**
 * @brief Busca o caminho completo de um comando
 * 
 * TODO (Pessoa B): Implementar
 * - Se cmd contém '/', usar ele diretamente
 * - Senão, buscar PATH na env
 * - Split PATH por ':'
 * - Para cada diretório, testar se existe cmd executável (access)
 * - Retornar o primeiro caminho válido
 * 
 * EXEMPLO:
 * cmd = "ls"
 * PATH = "/usr/bin:/bin"
 * Testar: /usr/bin/ls (OK!) -> retornar "/usr/bin/ls"
 * 
 * @param cmd Nome do comando
 * @param env Lista de ambiente
 * @return Caminho completo ou NULL se não encontrar
 */
char	*find_command_path(char *cmd, t_env *env)
{
	char	*path_value;
	char	**dirs;

	if (!cmd || cmd[0] == '\0')
		return (NULL);
    if (ft_strchr(cmd, '/'))
    	return (ft_strdup(cmd));
	path_value = get_path_value(env);
	if (!path_value)
		return (NULL);
	dirs = ft_split(path_value, ':');
	if (!dirs)
	   	return (NULL);
	return (search_in_dirs(dirs, cmd));
}
