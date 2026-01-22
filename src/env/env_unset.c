/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/01/21 20:21:33 by mona             ###   ########.fr       */
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
	// TODO: Implementar
	(void)env;
	(void)key;
	return (ERROR);
}
