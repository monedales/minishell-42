/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <maria-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/02/10 20:14:18 by maria-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Update the value of an existing variable
 * 
 * TODO: Implementar
 * - Dar free no value antigo
 * - Duplicar o novo value
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
 * @brief Define ou atualiza uma variável de ambiente
 * 
 * TODO: Implementar lógica completa
 * - Buscar se a key já existe na lista
 * - Se existe: atualizar o value
 * - Se não existe: criar novo nó e adicionar à lista
 * 
 * EXEMPLOS:
 * set_env_value(&env, "VAR", "42")
 *   -> Se VAR existe: atualiza valor para "42"
 *   -> Se VAR não existe: cria novo nó VAR=42
 * 
 * USADO PELO BUILTIN EXPORT!
 * 
 * @param env Ponteiro para o início da lista
 * @param key Nome da variável
 * @param value Valor da variável
 * @return SUCCESS ou ERROR
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
