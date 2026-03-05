/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <maria-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 17:30:48 by maria-ol          #+#    #+#             */
/*   Updated: 2026/02/25 17:54:28 by maria-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Checks if a string is enclosed in single quotes.
 *
 * Single quotes disable all variable expansion. Double quotes allow expansion.
 *
 * @param str String to check
 * @return TRUE if single quoted, FALSE otherwise
 */
int	is_single_quoted(char *str)
{
	size_t			i;
	t_quote_state	state;

	if (!str)
		return (FALSE);
	i = 0;
	if (str[0] != '\'')
		return (FALSE);
	state = QUOTE_NONE;
	while (str[i])
	{
		update_quote_state(str[i], &state);
		i++;
	}
	if (state == QUOTE_NONE && str[i - 1] == '\'')
		return (TRUE);
	else
		return (FALSE);
}

/**
 * @brief Checks if a character starts a variable ($VAR or $?).
 *
 * Returns TRUE if '$' followed by alphanumeric or '_', or special case '$?'.
 *
 * @param c Current character
 * @param next Next character
 * @return TRUE if variable start, FALSE otherwise
 */
int	is_var_start(char c, char next)
{
	if (c != '$')
		return (FALSE);
	if (next == '?')
		return (TRUE);
	if (ft_isalnum(next) == 1 || next == '_')
		return (TRUE);
	return (FALSE);
}

/**
 * @brief Extracts the variable name from a string after '$'.
 *
 * Handles alphanumeric and underscore, special case for '?'.
 *
 * @param str String after '$'
 * @param len Pointer to store extracted length
 * @return Extracted variable name (malloc'd)
 */
char	*extract_var_name(char *str, int *len)
{
	size_t	i;
	char	*s1;
	char	*var_name;

	if (!str || !len)
		return (NULL);
	i = 0;
	if (str[0] == '?')
	{
		s1 = ft_strdup("?");
		*len = 1;
		return (s1);
	}
	if (!is_var_start('$', str[0]))
		return (NULL);
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	*len = i;
	var_name = ft_calloc(*len + 1, sizeof(char));
	if (!var_name)
		return (NULL);
	ft_strlcpy(var_name, str, i + 1);
	return (var_name);
}

/*
** @brief Expands a variable and returns the expanded value.
**
** If it's $?, returns the exit status. Otherwise, searches in the environment.
**
** @param var_name Name of the variable
** @param mini Main structure
** @return Expanded value (malloc'd), or empty string if not found
*/
char	*expand_var_value(const char *var_name, t_mini *mini)
{
	char	*value;

	if (!ft_strncmp(var_name, "?", 2))
		return (ft_itoa(mini->last_exit_status));
	value = get_env_value(mini->env, var_name);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}
