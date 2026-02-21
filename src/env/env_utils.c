/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <maria-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/02/09 18:40:53 by maria-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Counts the number of variables in the environment list
 * 
 * @param env Environment list
 * @return Number of variables
 */
static int	count_env_vars(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

/**
 * @brief Converts the environment linked list to an array of strings
 * 
 * Creates a NULL-terminated array of "KEY=VALUE" strings.
 * Used by execve, which requires a char **envp in the traditional format.
 * 
 * @param env Environment list
 * @return Array of strings (must be freed after use) or NULL on error
 */
char	**env_to_array(t_env *env)
{
	int		count;
	int		idx;
	char	**arr;
	char	*tmp;

	idx = 0;
	count = count_env_vars(env);
	arr = malloc(sizeof(char *) * (count + 1));
	if (!arr)
		return (NULL);
	while (env)
	{
		tmp = ft_strjoin(env->key, "=");
		arr[idx] = ft_strjoin(tmp, env->value);
		free(tmp);
		if (!arr[idx])
			return (NULL);
		env = env->next;
		idx++;
	}
	arr[count] = NULL;
	return (arr);
}

/**
 * @brief Prints all environment variables
 * 
 * Traverses the list and prints each variable in "KEY=VALUE" format.
 * Used by the env builtin command.
 * 
 * @param env Environment list
 */

// trocar printf por ft_printf
void	print_env(t_env *env)
{
	while (env)
	{
		printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
}
