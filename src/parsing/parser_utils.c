/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <maria-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 00:00:00 by maria-ol          #+#    #+#             */
/*   Updated: 2026/02/13 20:11:25 by maria-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Creates a new command node with initialized fields
 *
 * @return New command or NULL if allocation fails
 */
t_cmd	*create_cmd_node(void)
{
	t_cmd	*comm;

	comm = malloc(sizeof(t_cmd));
	if (!comm)
		return (NULL);
	comm->args = NULL;
	comm->redirs = NULL;
	comm->pid = 0;
	comm->next = NULL;
	return (comm);
}

/**
 * @brief Adds a command node to the end of the command list
 *
 * @param head Pointer to the head of the command list
 * @param new Command node to add
 */
void	add_cmd(t_cmd **head, t_cmd *new)
{
	t_cmd	*current;

	if (!head || !new)
		return ;
	if (*head == NULL)
	{
		*head = new;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new;
}

/**
 * @brief Creates a new redirection node
 *
 * TODO: Implementar
 * - Allocate memory for t_redir
 * - Initialize type and file
 * - Initialize next as NULL
 *
 * @param type Type of redirection
 * @param file File name
 * @return New redirection or NULL on error
 */
t_redir	*create_redir_node(t_token_type type, char *file)
{
	(void)type;
	(void)file;
	return (NULL);
}

/**
 * @brief Adds an argument to the arguments array
 *
 * TODO: Implementar
 * - Reallocate args array to fit one more element
 * - Add the new argument
 * - Keep array NULL-terminated
 *
 * EXAMPLE:
 * Before: ["ls", NULL]
 * After: ["ls", "-la", NULL]
 *
 * @param cmd Current command
 * @param arg Argument to add
 * @return SUCCESS or ERROR
 */
int	add_arg_to_cmd(t_cmd *cmd, char *arg)
{
	(void)cmd;
	(void)arg;
	return (ERROR);
}

/**
 * @brief Adds a redirection to the list
 *
 * TODO: Implementar
 * - Add to the end of redirections list
 *
 * @param cmd Current command
 * @param redir Redirection to add
 */
void	add_redir_to_cmd(t_cmd *cmd, t_redir *redir)
{
	(void)cmd;
	(void)redir;
}
