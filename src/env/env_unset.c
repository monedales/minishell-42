/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <maria-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/02/10 20:53:09 by maria-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Remove uma variável de ambiente da lista
 * 
 * TODO: Implementar remoção de nó em lista ligada
 * - Buscar o nó com a key especificada
 * - Ajustar os ponteiros (cuidado com head!)
 * - Dar free em key, value e no nó
 * 
 * CASOS:
 * 1. Remover o primeiro nó (head)
 * 2. Remover nó do meio
 * 3. Remover último nó
 * 4. Key não existe (não fazer nada)
 * 
 * USADO PELO BUILTIN UNSET!
 * 
 * @param env Ponteiro para o início da lista
 * @param key Nome da variável a remover
 * @return SUCCESS se removeu, ERROR se não encontrou
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
