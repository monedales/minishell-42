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
	typedef struct s_cmd
	{
		char			**args;		// comando + argumentos [cmd, arg1, arg2, NULL]
		t_redir			*redirs;	// lista de redirecionamentos
		struct s_cmd	*next;		// próximo comando (após pipe)
	}	t_cmd;


int	execute_cmd_list(t_cmd *cmd_list, t_mini *mini)
{
	char	*command;

	command = cmd_list->args[0];
	if (is_builtin(command) == 1)
		execute_builtin(cmd_list, command);
	else if (command && cmd_list->next == NULL)
		execute_simple_cmd(cmd_list, mini);
	else
		execute_pipeline(cmd_list, mini)
	return (0);
}
