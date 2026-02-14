/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 00:00:00 by maria-ol          #+#    #+#             */
/*   Updated: 2026/02/14 14:03:26 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Frees the redirection list
 *
 * TODO: Implementar
 * - Iterate through list
 * - Free file
 * - Free node
 *
 * @param redirs Redirection list
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
 * @brief Frees the entire command list
 *
 * TODO: Implementar
 * - Iterate through command list
 * - For each command:
 *   - Free args array (each string + the array)
 *   - Free redirection list
 *   - Free the command node
 *
 * @param cmd_list Command list
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
