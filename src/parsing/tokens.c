/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 01:44:43 by maria-ol          #+#    #+#             */
/*   Updated: 2026/02/12 18:05:36 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Creates a new token node
 * 
 * Allocates a t_token, sets its type and duplicates the value
 * string (if provided). Initializes next pointer to NULL.
 * 
 * @param type Token type (WORD, PIPE, REDIR, etc.)
 * @param value Token string value, or NULL for operators
 * @return New token, or NULL on allocation failure
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
 * @brief Appends a token to the end of the linked list
 * 
 * If the list is empty, the new token becomes the head.
 * Otherwise, traverses to the last node and appends it.
 * 
 * @param head Pointer to the head of the token list
 * @param new_token Token to append
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
 * @brief Frees the entire token linked list
 * 
 * Iterates through the list, freeing the value string
 * and the node itself for each token.
 * 
 * @param tokens Head of the token list
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
