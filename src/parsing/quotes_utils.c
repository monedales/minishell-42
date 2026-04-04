/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 15:14:26 by mona              #+#    #+#             */
/*   Updated: 2026/04/04 15:20:01 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
 * @brief Removes quotes from all redirection filenames in a command list.
 *
 * Iterates every command's redir list and strips quotes from each
 * filename, except heredoc delimiters (already handled separately
 * during heredoc collection). Called after expansion so that quoted
 * metacharacters are preserved through the expansion stage.
 *
 * @param cmd_list Head of the command list to process.
 */
void	remove_quotes_from_redirs(t_cmd *cmd_list)
{
	t_cmd	*cmd;
	t_redir	*redir;
	char	*clean;

	cmd = cmd_list;
	while (cmd)
	{
		redir = cmd->redirs;
		while (redir)
		{
			if (redir->type != TKN_REDIR_HEREDOC && redir->file)
			{
				clean = remove_quotes(redir->file);
				if (clean)
				{
					free(redir->file);
					redir->file = clean;
				}
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
}

/**
 * @brief Removes quotes from all argument strings in a command list.
 *
 * Walks every command node and strips quotes from each argument in
 * the args array. Called after expansion so that quoted characters
 * (e.g. spaces, dollars) are preserved through earlier stages.
 *
 * @param cmd_list Head of the command list to process.
 */
void	remove_quotes_from_args(t_cmd *cmd_list)
{
	t_cmd	*cmd;
	char	*clean;
	int		i;

	cmd = cmd_list;
	while (cmd)
	{
		i = 0;
		while (cmd->args && cmd->args[i])
		{
			clean = remove_quotes(cmd->args[i]);
			if (clean)
			{
				free(cmd->args[i]);
				cmd->args[i] = clean;
			}
			i++;
		}
		cmd = cmd->next;
	}
}
