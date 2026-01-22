/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_get.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/01/21 20:21:33 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Busca o valor de uma variável de ambiente
 * 
 * TODO: Implementar busca na lista ligada
 * - Percorrer lista comparando keys
 * - Retornar o value se encontrar
 * - Retornar NULL se não encontrar
 * 
 * EXEMPLO:
 * get_env_value(env, "HOME") -> "/Users/mona"
 * get_env_value(env, "NONEXISTENT") -> NULL
 * 
 * IMPORTANTE: Esta função NÃO deve duplicar a string!
 * Retorna um ponteiro direto para o value dentro do nó.
 * 
 * @param env Lista de ambiente
 * @param key Nome da variável a buscar
 * @return Valor da variável ou NULL se não existir
 */
char	*get_env_value(t_env *env, const char *key)
{
	// TODO: Implementar
	(void)env;
	(void)key;
	return (NULL);
}
