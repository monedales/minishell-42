/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <maria-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/02/13 20:09:23 by maria-ol         ###   ########.fr       */
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
