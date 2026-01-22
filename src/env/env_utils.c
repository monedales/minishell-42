/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/01/21 20:21:33 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Conta o número de variáveis na lista
 * 
 * TODO: Implementar contagem
 * 
 * @param env Lista de ambiente
 * @return Número de variáveis
 */
static int	count_env_vars(t_env *env)
{
	// TODO: Implementar
	(void)env;
	return (0);
}

/**
 * @brief Converte lista ligada de ambiente em array de strings
 * 
 * TODO: Implementar conversão
 * - Contar número de variáveis
 * - Alocar array com tamanho suficiente (+1 para NULL)
 * - Para cada nó, criar string "KEY=VALUE"
 * - Adicionar ao array
 * - Terminar com NULL
 * 
 * EXEMPLO:
 * Lista: [HOME=/Users/mona] -> [USER=mona] -> NULL
 * 
 * Array: ["HOME=/Users/mona", "USER=mona", NULL]
 * 
 * IMPORTANTE: Esta função é usada pelo execve!
 * O execve precisa de um char **envp no formato tradicional.
 * 
 * @param env Lista de ambiente
 * @return Array de strings (precisa dar free depois)
 */
char	**env_to_array(t_env *env)
{
	// TODO: Implementar
	(void)env;
	return (NULL);
}

/**
 * @brief Imprime todas as variáveis de ambiente
 * 
 * TODO: Implementar impressão
 * - Percorrer lista
 * - Para cada nó, imprimir "KEY=VALUE\n"
 * 
 * USADO PELO BUILTIN ENV!
 * 
 * @param env Lista de ambiente
 */
void	print_env(t_env *env)
{
	// TODO: Implementar
	(void)env;
}
