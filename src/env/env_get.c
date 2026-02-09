/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_get.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <maria-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/02/09 18:03:08 by maria-ol         ###   ########.fr       */
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
