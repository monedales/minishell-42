/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <maria-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/02/11 02:39:09 by maria-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	if (str[*i] == '|')
		return (TOKEN_PIPE);
	else if (str[*i] == '<')
	{
		if (str[*i + 1] == '<')
		{
			(*i)++;
			return (TOKEN_REDIR_HEREDOC);
		}
		else
			return (TOKEN_REDIR_IN);
	}
	else if (str[*i] == '>')
	{
		if (str[*i + 1] == '>')
		{
			(*i)++;
			return (TOKEN_REDIR_APPEND);
		}
		else
			return (TOKEN_REDIR_OUT);
	}
	else
		return (TOKEN_WORD);
}

static int	is_delimiter(char c)
{
	return (c == ' ' || c == '\t' || c == '|'
		|| c == '<' || c == '>');
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
	int		start;
	int		in_single;
	int		in_double;
	char	*word;

	start = *i;
	in_single = 0;
	in_double = 0;
	while (str[*i] && (in_single || in_double || !is_delimiter(str[*i])))
	{
		if (str[*i] == '\'' && !in_double)
			in_single = !in_single;
		else if (str[*i] == '"' && !in_single)
			in_double = !in_double;
		(*i)++;
	}
	word = ft_substr(str, start, *i - start);
	if (!word)
		return (NULL);
	return (word);
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
	t_token			*head;
	t_token			*token;
	t_token_type	type;
	char			*value;
	int				i;

	if (!input)
		return (NULL);
	head = NULL;
	i = 0;
	while (input[i])
	{
		while (input[i] == ' ' || input[i] == '\t')
			i++;
		if (!input[i])
			break ;
		type = identify_token_type(input, &i);
		if (type != TOKEN_WORD)
		{
			token = create_token(type, NULL);
			i++;
		}
		else
		{
			value = extract_word(input, &i);
			if (!value)
			{
				free_tokens(head);
				return (NULL);
			}
			token = create_token(type, value);
			free(value);
		}
		if (!token)
		{
			free_tokens(head);
			return (NULL);
		}
		add_token(&head, token);
	}
	return (head);
}
