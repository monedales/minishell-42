/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by pessoa-b          #+#    #+#             */
/*   Updated: 2026/01/21 20:21:33 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Executa a lista de comandos (dispatcher principal)
 * 
 * TODO (Pessoa B): Implementar
 * - Se houver apenas 1 comando e for builtin: executar no processo pai
 * - Se houver apenas 1 comando: execute_simple_cmd
 * - Se houver múltiplos comandos (pipes): execute_pipeline
 * 
 * @param cmd_list Lista de comandos a executar
 * @param mini Estrutura principal
 * @return Exit status do último comando
 */
int	execute_cmd_list(t_cmd *cmd_list, t_mini *mini)
{
	// TODO: Implementar
	(void)cmd_list;
	(void)mini;
	return (0);
}
