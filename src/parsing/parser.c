/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/02/14 17:37:17 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks for syntax errors related to pipes in the token list.
 *
 * Detects if a pipe is at the end of the input or if two pipes appear 
 * consecutively.
 * Prints a syntax error message if an error is found.
 *
 * @param current The current token to check.
 * @return TRUE if a pipe error is found, otherwise FALSE.
 */
static int	is_pipe_error(t_token *current)
{
	if (current->type == TOKEN_PIPE)
	{
		if (!current->next || current->next->type == TOKEN_PIPE)
		{
			handle_error(ERR_SYNTAX, NULL, "'|'");
			return (TRUE);
		}
	}
	return (FALSE);
}

/**
 * @brief Checks for syntax errors related to redirections in the token list.
 *
 * Detects if a redirection is at the end of the input or not followed by a 
 * word token.
 * Prints a syntax error message if an error is found.
 *
 * @param current The current token to check.
 * @return TRUE if a redirection error is found, otherwise FALSE.
 */
static int	is_redir_error(t_token *curr)
{
	if (curr->type == TKN_REDIR_IN || curr->type == TKN_REDIR_OUT
		|| curr->type == TKN_REDIR_APPEND || curr->type == TKN_REDIR_HEREDOC)
	{
		if (!curr->next)
		{
			handle_error(ERR_SYNTAX, NULL, "'newline'");
			return (TRUE);
		}
		if (curr->next->type != TOKEN_WORD)
		{
			handle_error(ERR_SYNTAX, NULL, curr->next->value);
			return (TRUE);
		}
	}
	return (FALSE);
}

/**
 * @brief Validates the syntax of the token list for common shell errors.
 *
 * Checks for:
 * - Pipe at the beginning
 * - Pipe at the end
 * - Consecutive pipes
 * - Redirection without a following word
 * - Invalid token sequences
 * Prints a syntax error message if an error is found.
 *
 * @param tokens The list of tokens to validate.
 * @return TRUE if the syntax is valid, FALSE if an error is found.
 */
static int	validate_syntax(t_token *tokens)
{
	t_token	*current;

	if (tokens && tokens->type == TOKEN_PIPE)
	{
		handle_error(ERR_SYNTAX, NULL, "'|'");
		return (FALSE);
	}
	current = tokens;
	while (current)
	{
		if (is_pipe_error(current) || is_redir_error(current))
			return (FALSE);
		current = current->next;
	}
	return (TRUE);
}

/**
 * @brief Parses the token list and builds the command list structure.
 *
 * Iterates through the tokens, creating command and redirection nodes as needed.
 * Handles arguments, redirections, and pipes, and links commands together.
 *
 * @param curr The current token in the list.
 * @param cmd_list Pointer to the head of the command list.
 * @param current_cmd Pointer to the current command being built.
 */
static void	parse_tokens(t_token *curr, t_cmd **cmd_list, t_cmd **current_cmd)
{
	t_redir	*redir;

	redir = NULL;
	while (curr)
	{
		if (curr->type == TOKEN_WORD)
		{
			if (*current_cmd == NULL)
			{
				*current_cmd = create_cmd_node();
				add_cmd(cmd_list, *current_cmd);
			}
			add_arg_to_cmd(*current_cmd, curr->value);
		}
		else if (curr->type == TKN_REDIR_APPEND || curr->type == TKN_REDIR_IN
			|| curr->type == TKN_REDIR_HEREDOC || curr->type == TKN_REDIR_OUT)
		{
			redir = create_redir_node(curr->type, curr->next->value);
			add_redir_to_cmd(*current_cmd, redir);
			curr = curr->next;
		}
		else if (curr && curr->type == TOKEN_PIPE)
			*current_cmd = NULL;
		curr = curr->next;
	}
}

/**
 * @brief Main parser function that builds the command list from tokens.
 *
 * Validates the syntax of the token list and, if valid, constructs the
 * linked list of commands and their associated arguments and redirections.
 *
 * @param tokens The list of expanded tokens to parse.
 * @return Pointer to the head of the command list, or NULL on error.
 */
t_cmd	*parser(t_token *tokens)
{
	t_cmd	*cmd_list;
	t_cmd	*current_cmd;

	if (!tokens)
		return (NULL);
	if (validate_syntax(tokens) == FALSE)
		return (NULL);
	cmd_list = NULL;
	current_cmd = NULL;
	parse_tokens(tokens, &cmd_list, &current_cmd);
	return (cmd_list);
}
