/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <maria-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/03/13 21:32:06 by maria-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
 * @brief Processes one character of the string during variable expansion.
 *
 * Handles three cases: quote character (updates state and copies it),
 * variable start (expands $VAR or $?), or regular character (copies as-is).
 * Updates the expansion state in place via the t_exp_state struct.
 *
 * @param s    Expansion state (result buffer, index, quote state)
 * @param str  Original string being expanded
 * @param mini Main shell structure for env and exit status
 */
static void	process_char(t_exp_state *s, char *str, t_mini *mini)
{
	char	*value;

	if ((str[s->i] == '\'' && (s->state == QUOTE_NONE
				|| s->state == QUOTE_SINGLE))
		|| (str[s->i] == '"' && (s->state == QUOTE_NONE
				|| s->state == QUOTE_DOUBLE)))
	{
		update_quote_state(str[s->i], &s->state);
		s->result = append_char_to_buffer(s->result, str[s->i++]);
		return ;
	}
	if (s->state != QUOTE_SINGLE && is_var_start(str[s->i], str[s->i + 1]))
	{
		value = expand_one_var(&str[s->i], mini, &s->i);
		s->result = append_to_buffer(s->result, value);
		free(value);
		return ;
	}
	s->result = append_char_to_buffer(s->result, str[s->i++]);
}

/**
 * @brief Expands all variables in a string, except inside single quotes.
 *
 * Iterates through the string character by character using process_char,
 * expanding $VAR and $? while tracking quote state. Single quotes disable
 * expansion; double quotes allow only variable expansion.
 *
 * @param str  Original string to expand
 * @param mini Main structure (for env and exit_status)
 * @return New string with variables expanded (malloc'd), or NULL on error.
 */
char	*expand_string(char *str, t_mini *mini)
{
	t_exp_state	s;

	if (!str || !mini)
		return (NULL);
	s.result = NULL;
	s.i = 0;
	s.state = QUOTE_NONE;
	while (str[s.i])
		process_char(&s, str, mini);
	return (s.result);
}

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
char	*append_char_to_buffer(char *base, char c)
{
	char	tmp[2];

	tmp[0] = c;
	tmp[1] = '\0';
	return (append_to_buffer(base, tmp));
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
