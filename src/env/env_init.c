/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <maria-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/02/09 17:53:49 by maria-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Cria um novo nó de variável de ambiente
 * 
 * TODO: Implementar
 * - Alocar memória para t_env
 * - Duplicar key e value
 * - Inicializar next como NULL
 * 
 * @param key Nome da variável
 * @param value Valor da variável
 * @return Novo nó ou NULL em caso de erro
 */
static t_env	*create_env_node(char *key, char *value)
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
 * @brief Adiciona nó ao final da lista de ambiente
 * 
 * TODO: Implementar
 * - Se lista vazia, new vira o head
 * - Senão, percorrer até o final e adicionar
 * 
 * @param head Ponteiro para o início da lista
 * @param new Novo nó a adicionar
 */
static void	add_env_node(t_env **head, t_env *new)
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
 * @brief Separa uma string "KEY=VALUE" em key e value
 * 
 * TODO: Implementar parsing
 * - Encontrar o '='
 * - Extrair tudo antes como key
 * - Extrair tudo depois como value
 * 
 * EXEMPLO:
 * Input: "HOME=/Users/mona"
 * Output: key="HOME", value="/Users/mona"
 * 
 * @param env_str String no formato KEY=VALUE
 * @param key Ponteiro para armazenar a key
 * @param value Ponteiro para armazenar o value
 * @return SUCCESS ou ERROR
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
 * @brief Inicializa a lista de ambiente a partir do envp
 * 
 * ESTRATÉGIA:
 * 1. Criar lista ligada vazia
 * 2. Para cada string em envp:
 *    - Separar em key=value
 *    - Criar nó t_env
 *    - Adicionar à lista
 * 3. Retornar o head da lista
 * 
 * EXEMPLO:
 * envp[0] = "HOME=/Users/mona"
 * envp[1] = "PATH=/usr/bin:/bin"
 * envp[2] = "USER=mona"
 * envp[3] = NULL
 * 
 * Resultado:
 * [HOME=/Users/mona] -> [PATH=/usr/bin:/bin] -> [USER=mona] -> NULL
 * 
 * @param envp Array de strings do main
 * @return Lista ligada de ambiente ou NULL em caso de erro
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
 * @brief Libera toda a lista de ambiente
 * 
 * TODO: Implementar
 * - Percorrer lista
 * - Dar free em key e value
 * - Dar free no nó
 * 
 * @param env Lista de ambiente
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
