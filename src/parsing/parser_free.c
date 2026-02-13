/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <maria-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 00:00:00 by maria-ol          #+#    #+#             */
/*   Updated: 2026/02/13 20:11:25 by maria-ol         ###   ########.fr       */
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
	(void)redirs;
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
	(void)cmd_list;
}
