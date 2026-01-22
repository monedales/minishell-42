/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/01/21 20:21:33 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Atualiza o valor de uma variável existente
 * 
 * TODO: Implementar
 * - Dar free no value antigo
 * - Duplicar o novo value
 * 
 * @param node Nó a ser atualizado
 * @param new_value Novo valor
 * @return SUCCESS ou ERROR
 */
static int	update_env_node(t_env *node, const char *new_value)
{
	// TODO: Implementar
	(void)node;
	(void)new_value;
	return (ERROR);
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
	// TODO: Implementar
	(void)env;
	(void)key;
	(void)value;
	return (ERROR);
}
