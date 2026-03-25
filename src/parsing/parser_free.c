/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 00:00:00 by maria-ol          #+#    #+#             */
/*   Updated: 2026/03/24 21:39:58 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Ensures a current command node exists, creating one if needed.
 *
 * Used before adding args or redirs to guarantee *current_cmd is not NULL.
 * This handles cases where a redirection appears before the command name
 * (e.g. "> out.txt ls") and no cmd node has been created yet.
 *
 * @param cmd_list    Head of the command list
 * @param current_cmd Pointer to the current command being built
 */
void	ensure_cmd(t_cmd **cmd_list, t_cmd **current_cmd)
{
	if (*current_cmd == NULL)
	{
		*current_cmd = create_cmd_node();
		add_cmd(cmd_list, *current_cmd);
	}
}

/**
 * @brief Frees the redirection list.
 *
 * Iterates through the linked list of redirections,
 * freeing the file string and the node itself.
 *
 * @param redirs Head of the redirection linked list.
 */
void	free_redirs(t_redir *redirs)
{
	t_redir	*tmp;

	while (redirs != NULL)
	{
		tmp = redirs->next;
		free(redirs->file);
		free(redirs);
		redirs = tmp;
	}
}

/**
 * @brief Frees the entire command list.
 *
 * Iterates through the linked list of commands. For each node,
 * frees every string in the args array, the array itself,
 * the redirection list via free_redirs, and the node.
 *
 * @param cmd_list Head of the command linked list.
 */
void	free_cmd_list(t_cmd *cmd_list)
{
	t_cmd	*tmp;
	int		i;

	while (cmd_list != NULL)
	{
		tmp = cmd_list->next;
		if (cmd_list->args)
		{
			i = 0;
			while (cmd_list->args[i])
			{
				free(cmd_list->args[i]);
				i++;
			}
			free(cmd_list->args);
		}
		free_redirs(cmd_list->redirs);
		free(cmd_list);
		cmd_list = tmp;
	}
}
