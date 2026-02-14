/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <maria-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 00:00:00 by maria-ol          #+#    #+#             */
/*   Updated: 2026/02/13 22:01:03 by maria-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Creates a new command node
 * 
 * Creates a new command node with initialized fields
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
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->file = ft_strdup(file);
	if (!redir->file)
	{
		free(redir);
		return (NULL);
	}
	redir->type = type;
	redir->next = NULL;
	return (redir);
}

/**
 * @brief Counts the number of arguments in an array
 *
 * @param args Array of arguments
 * @return Number of arguments (excluding NULL)
 */
int	count_args(char **args)
{
	int	i;

	i = 0;
	if (args)
	{
		while (args[i])
			i++;
	}
	return (i);
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
	t_redir	*current;

	if (!cmd || !redir)
		return ;
	if (cmd->redirs == NULL)
	{
		cmd->redirs = redir;
		return ;
	}
	current = cmd->redirs;
	while (current->next)
		current = current->next;
	current->next = redir;
}
