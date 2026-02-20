/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <maria-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 00:00:00 by maria-ol          #+#    #+#             */
/*   Updated: 2026/02/19 22:10:28 by maria-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Creates a new command node with initialized fields
 *
 * Allocates memory for a new command structure and initializes all fields
 * to their default values (NULL for pointers, 0 for pid). Used when
 * starting to parse a new command in the pipeline.
 *
 * @return Pointer to new command node, or NULL if allocation fails
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
 * Traverses the command list to find the last node and appends the new
 * command. If the list is empty, the new command becomes the head.
 * Used to build the pipeline by linking commands separated by pipes.
 *
 * @param head Pointer to the head of the command list
 * @param new Command node to add to the end
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
 * Allocates memory for a new redirection structure and duplicates the
 * filename string. The redirection type can be input (<), output (>),
 * append (>>), or heredoc (<<). Used when parsing redirection tokens.
 *
 * @param type Type of redirection (TKN_REDIR_IN/OUT/APPEND/HEREDOC)
 * @param file Filename for the redirection
 * @return Pointer to new redirection node, or NULL if allocation fails
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
 * @brief Adds a redirection to the end of the redirections list
 *
 * Traverses the command's redirection list to find the last node and
 * appends the new redirection. If the list is empty, the new redirection
 * becomes the first. Multiple redirections can be chained for the same
 * command (e.g., "< in.txt > out.txt").
 *
 * @param cmd Current command structure
 * @param redir Redirection node to add to the list
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

/**
 * @brief Adds an argument to the command's argument array.
 *
 * Dynamically reallocates the args array to add a new argument,
 * ensuring the array remains NULL-terminated.
 *
 * @param cmd The command to which the argument will be added.
 * @param arg The argument string to add.
 * @return SUCCESS on success, ERROR on allocation failure.
 */
int	add_arg_to_cmd(t_cmd *cmd, char *arg)
{
	char	**new_args;
	int		i;
	int		j;

	i = count_args(cmd->args);
	new_args = malloc(sizeof(char *) * (i + 2));
	if (!new_args)
		return (ERROR);
	j = 0;
	while (j < i)
	{
		new_args[j] = cmd->args[j];
		j++;
	}
	new_args[i] = ft_strdup(arg);
	if (!new_args[i])
	{
		free(new_args);
		return (ERROR);
	}
	new_args[i + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
	return (SUCCESS);
}
