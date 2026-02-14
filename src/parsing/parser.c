/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <maria-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/02/13 22:01:23 by maria-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Validates the syntax of tokens
 * 
 * TODO: Implementar validação de erros comuns:
 * - Pipe no início ou no final
 * - Dois pipes seguidos (| |)
 * - Redirecionamento sem arquivo (< ou > ou >> sem word depois)
 * - Tokens inválidos
 * 
 * @param tokens Lista de tokens
 * @return TRUE se válido, FALSE se houver erro
 */
static int	validate_syntax(t_token *tokens)
{
	// TODO: Implementar
	(void)tokens;
	return (TRUE);
}

/**
 * @brief Adds an argument to the arguments array
 *
 * Reallocates the args array to fit one more element and adds the new
 * argument while keeping the array NULL-terminated.
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

/**
 * @brief Constrói a lista de comandos a partir dos tokens
 * 
 * ESTRATÉGIA:
 * 1. Validar sintaxe primeiro
 * 2. Percorrer tokens:
 *    - TOKEN_WORD: adicionar como argumento ao comando atual
 *    - TOKEN_REDIR_*: o próximo WORD é o arquivo, criar t_redir
 *    - TOKEN_PIPE: finalizar comando atual, criar novo
 * 3. Retornar lista ligada de comandos
 * 
 * EXEMPLO:
 * Tokens: [WORD:ls] [WORD:-la] [PIPE] [WORD:grep] [WORD:test]
 * 
 * Resultado:
 * cmd1: args=["ls", "-la", NULL], next=cmd2
 * cmd2: args=["grep", "test", NULL], next=NULL
 * 
 * EXEMPLO COM REDIRECIONAMENTO:
 * Tokens: [WORD:cat] [REDIR_IN] [WORD:file.txt] [PIPE] [WORD:wc]
 * 
 * Resultado:
 * cmd1: args=["cat", NULL], redirs=[<file.txt], next=cmd2
 * cmd2: args=["wc", NULL], redirs=NULL, next=NULL
 * 
 * @param tokens Lista de tokens expandidos
 * @return Lista de comandos ou NULL em caso de erro
 */
t_cmd	*parser(t_token *tokens)
{
	(void)tokens;
	return (NULL);
}
