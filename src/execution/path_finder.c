/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_finder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <maria-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by pessoa-b          #+#    #+#             */
/*   Updated: 2026/03/12 21:12:51 by maria-ol         ###   ########.fr       */
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
		if (access(full_path, X_OK) == 0)
		{
			free_array(dirs);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_array(dirs);
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
 * @brief Resolves the executable path for a command.
 *
 * Resolution order:
 * - If `cmd` is NULL/empty, returns NULL.
 * - If `cmd` contains '/', treats it as explicit path and duplicates it.
 * - Otherwise, gets `PATH` from `env`, splits by ':', and searches each
 *   directory for an executable file with `access(X_OK)`.
 *
 * @param cmd Command name or explicit path.
 * @param env Environment linked list.
 * @return Malloc'ed full path when found, or NULL if unresolved.
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
