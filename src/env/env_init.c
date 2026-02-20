/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <maria-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/02/10 19:50:38 by maria-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Creates a new environment variable node
 * 
 * Allocates memory for a t_env node, duplicates key and value,
 * and initializes prev and next pointers to NULL.
 * 
 * @param key Variable name
 * @param value Variable value
 * @return New node or NULL on error
 */
t_env	*create_env_node(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	if (!node->key)
	{
		free(node);
		return (NULL);
	}
	node->value = ft_strdup(value);
	if (!node->value)
	{
		free(node->key);
		return (NULL);
	}
	node->prev = NULL;
	node->next = NULL;

	return (node);
}

/**
 * @brief Adds a node to the end of the environment list
 * 
 * If the list is empty, the new node becomes the head.
 * Otherwise, traverses to the end and appends the node.
 * 
 * @param head Pointer to the head of the list
 * @param new New node to add
 */
void	add_env_node(t_env **head, t_env *new)
{
	t_env	*current;

	if (!head || !new)
		return ;
	if (*head == NULL)
	{
		*head = new;
		return ;
	}
	current = *head;
	while (current->next != NULL)
		current = current->next;
	current->next = new;
	new->prev = current;
}

/**
 * @brief Splits a "KEY=VALUE" string into key and value
 * 
 * Finds the first '=' and extracts everything before it as key
 * and everything after it as value.
 * 
 * Example: "HOME=/Users/mona" -> key="HOME", value="/Users/mona"
 * 
 * @param env_str String in KEY=VALUE format
 * @param key Pointer to store the key
 * @param value Pointer to store the value
 * @return SUCCESS or ERROR
 */
static int	split_env_string(char *env_str, char **key, char **value)
{
	int		pos;
	char	*equal;

	equal = ft_strchr(env_str, '=');
	if (!equal)
		return (ERROR);
	pos = equal - env_str;
	*key = ft_substr(env_str, 0, pos);
	if (!*key)
		return (ERROR);
	*value = ft_strdup(equal + 1);
	if (!*value)
	{
		free(*key);
		return (ERROR);
	}
	return (SUCCESS);
}

/**
 * @brief Initializes the environment list from envp
 * 
 * Iterates through the envp array, splits each string into
 * key=value, creates a node and adds it to the linked list.
 * 
 * @param envp Array of strings from main
 * @return Linked list of environment variables or NULL on error
 */
t_env	*init_env(char **envp)
{
	t_env	*head;
	t_env	*node;
	char	*key;
	char	*value;
	int		idx;

	if (!envp)
		return (NULL);
	head = NULL;
	key = NULL;
	value = NULL;
	idx = 0;
	while (envp[idx])
	{
		if (split_env_string(envp[idx], &key, &value) == SUCCESS)
		{
			node = create_env_node(key, value);
			free(key);
			free(value);
			if (node)
				add_env_node(&head, node);
		}
		idx++;
	}
	return (head);
}

/**
 * @brief Frees the entire environment list
 * 
 * Traverses the list, freeing key, value and the node itself.
 * 
 * @param env Environment list
 */
void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = tmp;
	}
}
