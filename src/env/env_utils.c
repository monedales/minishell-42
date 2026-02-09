/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <maria-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/02/09 18:39:27 by maria-ol         ###   ########.fr       */
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
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
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
	int		count;
	int		idx;
	char	**arr;
	char	*tmp;

	idx = 0;
	count = count_env_vars(env);
	arr = malloc(sizeof(char *) * (count + 1));
	if (!arr)
		return (NULL);
	while (env)
	{
		tmp = ft_strjoin(env->key, "=");
		arr[idx] = ft_strjoin(tmp, env->value);
		free(tmp);
		if (!arr[idx])
			return (NULL);
		env = env->next;
		idx++;
	}
	arr[count] = NULL;
	return (arr);
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
	while (env)
	{
		printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
}
