/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <maria-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/02/13 18:50:59 by maria-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Updates the quote state based on the current character
 *
 * Toggles between quote states (NONE, SINGLE, DOUBLE) when encountering
 * delimiter quotes. Single quotes are ignored inside double quotes and
 * vice versa.
 *
 * @param c Current character
 * @param state Pointer to the current quote state
 */
static void	update_quote_state(char c, t_quote_state *state)
{
	if (c == '"' && *state == QUOTE_NONE)
		*state = QUOTE_DOUBLE;
	else if (c == '"' && *state == QUOTE_DOUBLE)
		*state = QUOTE_NONE;
	else if (c == '\'' && *state == QUOTE_NONE)
		*state = QUOTE_SINGLE;
	else if (c == '\'' && *state == QUOTE_SINGLE)
		*state = QUOTE_NONE;
}

/**
 * @brief Identifies if a character at a certain position is inside quotes
 *
 * Traverses the string from the beginning to 'pos' tracking the quote state.
 * Quotes inside other quotes do not change the state.
 *
 * @param str Complete string
 * @param pos Position to check
 * @param state Pointer to store the current state
 * @return TRUE if quoted, FALSE otherwise
 */
int	is_in_quotes(char *str, int pos, t_quote_state *state)
{
	int	i;

	*state = QUOTE_NONE;
	i = 0;
	while (i < pos && str[i])
	{
		update_quote_state(str[i], state);
		i++;
	}
	return (*state != QUOTE_NONE);
}

/**
 * @brief Removes quotes from a string
 *
 * Traverses the string tracking quote state and copies characters
 * to a new string, except delimiter quotes. Content inside quotes
 * is kept intact. This function is called AFTER expansion.
 *
 * @param str String with quotes
 * @return New string without quotes (must be freed after use)
 */
char	*remove_quotes(char *str)
{
	char			*result;
	t_quote_state	state;
	size_t			i;
	size_t			j;

	if (!str)
		return (NULL);
	result = malloc(sizeof(char) * ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	state = QUOTE_NONE;
	i = 0;
	j = 0;
	while (str[i])
	{
		if ((str[i] == '"' && (state == QUOTE_NONE || state == QUOTE_DOUBLE))
			|| (str[i] == '\'' && (state == QUOTE_NONE
					|| state == QUOTE_SINGLE)))
			update_quote_state(str[i], &state);
		else
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

/**
 * @brief Checks if there are unclosed quotes
 *
 * Tracks opening and closing of quotes and returns an error
 * if there are unbalanced quotes.
 *
 * @param str String to be validated
 * @return TRUE if valid, FALSE if there's a mistake
 */
int	validate_quotes(char *str)
{
	int				i;
	t_quote_state	state;

	state = QUOTE_NONE;
	i = 0;
	while (str[i])
	{
		update_quote_state(str[i], &state);
		i++;
	}
	if (state != QUOTE_NONE)
		return (FALSE);
	return (TRUE);
}
