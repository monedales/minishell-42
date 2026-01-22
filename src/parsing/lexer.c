/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/01/21 20:21:33 by mona             ###   ########.fr       */
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
static t_token	*create_token(t_token_type type, char *value)
{
	// TODO: Implementar
	(void)type;
	(void)value;
	return (NULL);
}

/**
 * @brief Adiciona token ao final da lista
 * 
 * TODO: Implementar adição de token à lista ligada
 * 
 * @param head Ponteiro para o início da lista
 * @param new_token Token a ser adicionado
 */
static void	add_token(t_token **head, t_token *new_token)
{
	// TODO: Implementar
	(void)head;
	(void)new_token;
}

/**
 * @brief Identifica o tipo de token baseado no caractere
 * 
 * TODO: Implementar identificação de:
 * - '|' -> TOKEN_PIPE
 * - '<' -> TOKEN_REDIR_IN ou TOKEN_REDIR_HEREDOC (se '<<')
 * - '>' -> TOKEN_REDIR_OUT ou TOKEN_REDIR_APPEND (se '>>')
 * - outros -> TOKEN_WORD
 * 
 * @param str String atual
 * @param i Índice atual
 * @return Tipo de token identificado
 */
static t_token_type	identify_token_type(char *str, int *i)
{
	// TODO: Implementar
	(void)str;
	(void)i;
	return (TOKEN_WORD);
}

/**
 * @brief Extrai uma palavra (TOKEN_WORD) respeitando aspas
 * 
 * TODO: Implementar extração de palavra
 * - Enquanto não encontrar espaço, pipe ou redirecionamento
 * - Respeitar aspas (não parar em espaços dentro de aspas)
 * - Usar máquina de estados para rastrear se está dentro de aspas
 * 
 * @param str String de input
 * @param i Ponteiro para índice atual (será atualizado)
 * @return String com a palavra extraída
 */
static char	*extract_word(char *str, int *i)
{
	// TODO: Implementar
	(void)str;
	(void)i;
	return (NULL);
}

/**
 * @brief Tokeniza a string de input
 * 
 * ESTRATÉGIA:
 * 1. Percorrer a string caractere por caractere
 * 2. Ignorar espaços
 * 3. Identificar operadores (|, <, >, <<, >>)
 * 4. Extrair palavras (respeitando aspas)
 * 5. Criar tokens e adicionar à lista
 * 
 * EXEMPLO:
 * Input:  echo "hello world" | grep hello > out.txt
 * Output: [WORD:echo] [WORD:hello world] [PIPE] [WORD:grep] 
 *         [WORD:hello] [REDIR_OUT] [WORD:out.txt]
 * 
 * @param input String lida pelo readline
 * @return Lista ligada de tokens ou NULL em caso de erro
 */
t_token	*lexer(char *input)
{
	// TODO: Implementar lógica completa do lexer
	(void)input;
	return (NULL);
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
	// TODO: Implementar
	(void)tokens;
}
