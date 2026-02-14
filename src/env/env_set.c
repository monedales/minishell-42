/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/02/14 17:38:18 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Updates the value of an existing environment variable
 * 
 * Frees the old value and duplicates the new one.
 * 
 * @param node Node to be updated
 * @param new_value New value for existing variable
 * @return SUCCESS or ERROR
 */
static int	update_env_node(t_env *node, const char *new_value)
{
	free(node->value);
	node->value = ft_strdup(new_value);
	if (!node->value)
		return (ERROR);
	return (SUCCESS);
}

/**
 * @brief Sets or updates an environment variable
 * 
 * Searches for the key in the list. If found, updates the value.
 * If not found, creates a new node and adds it to the list.
 * Used by the export builtin command.
 * 
 * @param env Pointer to the head of the list
 * @param key Variable name
 * @param value Variable value
 * @return SUCCESS or ERROR
 */
int	set_env_value(t_env **env, const char *key, const char *value)
{
	t_env	*current;
	t_env	*node;

	if (!env || !key)
		return (ERROR);
	current = *env;
	while (current)
	{
		if (ft_strlen(current->key) == ft_strlen(key)
			&& ft_strncmp(current->key, key, ft_strlen(key)) == 0)
		{
			update_env_node(current, value);
			return (SUCCESS);
		}
		current = current->next;
	}
	node = create_env_node(key, value);
	if (!node)
		return (ERROR);
	add_env_node(env, node);
	return (SUCCESS);
}
