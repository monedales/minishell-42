/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <maria-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/02/10 21:08:46 by maria-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Removes an environment variable from the list
 * 
 * Searches for the node with the specified key, adjusts
 * prev/next pointers and frees the node.
 * Used by the unset builtin command.
 * 
 * @param env Pointer to the head of the list
 * @param key Variable name to remove
 * @return SUCCESS if removed, ERROR if not found
 */
int	unset_env_value(t_env **env, const char *key)
{
	t_env	*current;

	if (!env || !*env || !key)
		return (ERROR);
	current = *env;
	while (current)
	{
		if (ft_strlen(current->key) == ft_strlen(key)
			&& ft_strncmp(current->key, key, ft_strlen(key)) == 0)
		{
			if (current->prev == NULL)
				*env = current->next;
			else
				current->prev->next = current->next;
			if (current->next)
				current->next->prev = current->prev;
			free(current->key);
			free(current->value);
			free(current);
			return (SUCCESS);
		}	
		current = current->next;
	}
	return (ERROR);
}
