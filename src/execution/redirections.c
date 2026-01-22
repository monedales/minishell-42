/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by pessoa-b          #+#    #+#             */
/*   Updated: 2026/01/21 20:21:33 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Configura os redirecionamentos para um comando
 * 
 * TODO (Pessoa B): Implementar
 * - Percorrer lista de redirecionamentos
 * - Para cada tipo:
 *   - REDIR_IN (<): open arquivo, dup2 para stdin
 *   - REDIR_OUT (>): open/create arquivo, dup2 para stdout
 *   - REDIR_APPEND (>>): open/create em modo append, dup2 para stdout
 *   - REDIR_HEREDOC (<<): ler até delimiter, criar pipe temporário
 * - Fechar FDs originais após dup2
 * 
 * @param redirs Lista de redirecionamentos
 * @return SUCCESS ou ERROR
 */
int	setup_redirections(t_redir *redirs)
{
	// TODO: Implementar
	(void)redirs;
	return (SUCCESS);
}
