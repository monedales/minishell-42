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

// TODO: diminuir essa funçao 
char	*find_command_path(char *cmd_name, t_env *env_list)
{
	char	*path_value;
	char	**dir;
	char	*temp;
	char	*full_path;
	int		i;

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
			// free_array(dir);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	return (NULL);
}
