/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipeline.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by pessoa-b          #+#    #+#             */
/*   Updated: 2026/01/21 20:21:33 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Executa uma pipeline (comandos separados por pipes)
 * 
 * TODO (Pessoa B): Implementar
 * - Para cada comando:
 *   - Criar pipe
 *   - Fork
 *   - Conectar stdin/stdout adequadamente
 *   - Executar comando
 * - Fechar todos os FDs
 * - Esperar todos os processos
 * 
 * @param cmd_list Lista de comandos conectados por pipes
 * @param mini Estrutura principal
 * @return Exit status do último comando
 */
int	execute_pipeline(t_cmd *cmd_list, t_mini *mini)
{
	// TODO: Implementar
	(void)cmd_list;
	(void)mini;
	return (0);
}
