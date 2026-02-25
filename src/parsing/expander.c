/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <maria-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/02/25 18:02:26 by maria-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Adds a character to a dynamic buffer.
 *
 * Concatenates a single character to the given buffer, reallocating as needed.
 * Used to build expanded strings one character at a time.
 *
 * @param base Current buffer (can be NULL)
 * @param c Character to add
 * @return New buffer with character appended, or NULL on error.
 */
static char	*append_char_to_buffer(char *base, char c)
{
	char	tmp[2];

	tmp[0] = c;
	tmp[1] = '\0';
	return (append_to_buffer(base, tmp));
}

/**
 * @brief Expands a variable found in the string.
 *
 * Extracts the variable name, expands its value, and advances the index.
 * Handles special case for $?.
 *
 * @param str Pointer to start of $VAR
 * @param mini Main structure for env and exit status
 * @param i Pointer to index to advance
 * @return Expanded value (malloc'd), or NULL.
 */
static char	*expand_one_var(char *str, t_mini *mini, int *i)
{
	char	*var_name;
	char	*value;
	int		len;

	len = 0;
	var_name = extract_var_name(str + 1, &len);
	value = expand_var_value(var_name, mini);
	free(var_name);
	*i += len + 1;
	return (value);
}

/**
 * @brief Expands all variables in a string, except inside single quotes.
 *
 * Iterates through the string, expanding $VAR and $?, building a new string.
 * Single quotes disable expansion. Uses helpers for buffer and variable logic.
 *
 * Example:
 *   Input: "Hello $USER" with USER=mona -> "Hello mona"
 *   Input: "exit: $?" with status=127 -> "exit: 127"
 *   Input: 'Hello $USER' -> 'Hello $USER' (no expansion)
 *
 * @param str Original string
 * @param mini Main structure (for env and exit_status)
 * @return New string with variables expanded (malloc'd).
 */
char	*expand_string(char *str, t_mini *mini)
{
	char	*result;
	int		i;
	char	*value;

	if (!str || !mini)
		return (NULL);
	if (is_single_quoted(str))
		return (ft_strdup(str));
	result = NULL;
	i = 0;
	while (str[i])
	{
		if (is_var_start(str[i], str[i + 1]))
		{
			value = expand_one_var(&str[i], mini, &i);
			result = append_to_buffer(result, value);
			free(value);
		}
		else
			result = append_char_to_buffer(result, str[i++]);
	}
	return (result);
}

/**
 * @brief Concatenates two string pieces, reallocating the buffer.
 *
 * Receives a base buffer (can be NULL) and a piece to add.
 * Returns a new buffer with both pieces, freeing the old one.
 *
 * @param base Current buffer (can be NULL)
 * @param add Piece to add
 * @return New buffer, or NULL on error.
 */
static char	*append_to_buffer(char *base, const char *add)
{
	char	*newbuf;
	int		len_base;
	int		len_add;

	len_base = 0;
	len_add = 0;
	if (base)
		len_base = ft_strlen(base);
	if (add)
		len_add = ft_strlen(add);
	newbuf = ft_calloc(len_base + len_add + 1, sizeof(char));
	if (!newbuf)
	{
		free(base);
		return (NULL);
	}
	if (base)
		ft_strlcpy(newbuf, base, len_base + 1);
	if (add)
		ft_strlcat(newbuf, add, len_base + len_add + 1);
	free(base);
	return (newbuf);
}

/**
 * @brief Expands variables in tokens, including filenames after redirections.
 *
 * For each TOKEN_WORD not in single quotes, expands variables. For redirection
 * tokens, expands the filename if not in single quotes. 
 * Modifies tokens in-place.
 *
 * @param tokens List of tokens to expand (modified in-place)
 * @param mini Main structure with env and exit_status
 */
void	expand_tokens(t_token *tokens, t_mini *mini)
{
	t_token	*curr;
	char	*expanded;

	if (!tokens || !mini)
		return ;
	curr = tokens;
	while (curr)
	{
		if (curr->type == TOKEN_WORD && !is_single_quoted(curr->value))
		{
			expanded = expand_string(curr->value, mini);
			free(curr->value);
			curr->value = expanded;
		}
		else if ((curr->type == TKN_REDIR_HEREDOC || curr->type == TKN_REDIR_OUT
				|| curr->type == TKN_REDIR_APPEND || curr->type == TKN_REDIR_IN)
			&& curr->next && curr->next->type == TOKEN_WORD
			&& !is_single_quoted(curr->next->value))
		{
			expanded = expand_string(curr->next->value, mini);
			free(curr->next->value);
			curr->next->value = expanded;
		}
		curr = curr->next;
	}
}
