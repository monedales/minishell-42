/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/02/14 17:36:10 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Identifies the token type based on the current character
 * 
 * Checks for operator characters and returns the corresponding type:
 * - '|'  -> TOKEN_PIPE
 * - '<'  -> TKN_REDIR_IN or TKN_REDIR_HEREDOC ('<<')
 * - '>'  -> TKN_REDIR_OUT or TKN_REDIR_APPEND ('>>')
 * - else -> TOKEN_WORD
 * 
 * @param str Input string
 * @param i Pointer to current index (incremented for double operators)
 * @return The identified token type
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
			return (TKN_REDIR_HEREDOC);
		}
		else
			return (TKN_REDIR_IN);
	}
	else if (str[*i] == '>')
	{
		if (str[*i + 1] == '>')
		{
			(*i)++;
			return (TKN_REDIR_APPEND);
		}
		else
			return (TKN_REDIR_OUT);
	}
	else
		return (TOKEN_WORD);
}

/**
 * @brief Checks if a character is a token delimiter
 * 
 * Delimiters are whitespace, pipe and redirection characters.
 * 
 * @param c Character to check
 * @return 1 if delimiter, 0 otherwise
 */
static int	is_delimiter(char c)
{
	return (c == ' ' || c == '\t' || c == '|'
		|| c == '<' || c == '>');
}

/**
 * @brief Extracts a word token from the input string
 * 
 * Reads characters until a delimiter is found, while respecting
 * single and double quotes (delimiters inside quotes are ignored).
 * 
 * @param str Input string
 * @param i Pointer to current index (updated after extraction)
 * @return Newly allocated string with the extracted word, or NULL
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
 * @brief Creates the next token from the current position
 * 
 * Identifies the token type and creates either an operator token
 * (with NULL value) or a word token (with extracted string value).
 * 
 * @param input Input string
 * @param i Pointer to current index (updated after tokenization)
 * @return New token, or NULL on allocation failure
 */
static t_token	*next_token(char *input, int *i)
{
	t_token_type	type;
	t_token			*token;
	char			*value;

	type = identify_token_type(input, i);
	if (type != TOKEN_WORD)
	{
		token = create_token(type, NULL);
		(*i)++;
	}
	else
	{
		value = extract_word(input, i);
		if (!value)
			return (NULL);
		token = create_token(type, value);
		free(value);
	}
	return (token);
}

/**
 * @brief Tokenizes the input string into a linked list of tokens
 * 
 * Iterates through the input, skipping whitespace and creating tokens
 * for each word or operator found. Handles pipes, redirections,
 * and quoted strings.
 * 
 * @param input Raw input string from readline
 * @return Linked list of tokens, or NULL on error or empty input
 */
t_token	*lexer(char *input)
{
	t_token	*head;
	t_token	*token;
	int		i;

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
		token = next_token(input, &i);
		if (!token)
		{
			free_tokens(head);
			return (NULL);
		}
		add_token(&head, token);
	}
	return (head);
}
