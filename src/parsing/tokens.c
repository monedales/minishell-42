/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 01:44:43 by maria-ol          #+#    #+#             */
/*   Updated: 2026/02/12 17:48:13 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Cria um novo token
 * 
 * TODO: Implementar criação de token com malloc
 * - Alocar memória para t_token
 * - Inicializar type e value
 * - Inicializar next como NULL
 * 
 * @param type Tipo do token
 * @param value Valor do token (duplicar string)
 * @return Novo token ou NULL em caso de erro
 */
t_token	*create_token(t_token_type type, char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	if (value)
		token->value = ft_strdup(value);
	else
		token->value = NULL;
	token->next = NULL;
	return (token);
}

/**
 * @brief Adiciona token ao final da lista
 * 
 * TODO: Implementar adição de token à lista ligada
 * 
 * @param head Ponteiro para o início da lista
 * @param new_token Token a ser adicionado
 */
void	add_token(t_token **head, t_token *new_token)
{
	t_token	*current;

	if (!head || !new_token)
		return ;
	if (*head == NULL)
	{
		*head = new_token;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

/**
 * @brief Libera toda a lista de tokens
 * 
 * TODO: Implementar free recursivo ou iterativo
 * - Percorrer a lista
 * - Dar free no value de cada token
 * - Dar free no próprio token
 * 
 * @param tokens Cabeça da lista de tokens
 */
void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens->next;
		free(tokens->value);
		free(tokens);
		tokens = tmp;
	}
}
