/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_get.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <maria-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/02/09 18:07:13 by maria-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Searches for an environment variable value
 * 
 * Traverses the linked list comparing keys.
 * Returns a direct pointer to the value (does NOT duplicate it).
 * 
 * Example: get_env_value(env, "HOME") -> "/Users/mona"
 * 
 * @param env Environment list
 * @param key Variable name to search for
 * @return Variable value or NULL if not found
 */
char	*get_env_value(t_env *env, const char *key)
{
	if (!env || !key)
		return (NULL);
	while (env)
	{
		if (ft_strlen(env->key) == ft_strlen(key)
			&& ft_strncmp(env->key, key, ft_strlen(key)) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}
