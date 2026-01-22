/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/01/21 20:21:33 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Identifica se um caractere em certa posição está dentro de aspas
 * 
 * TODO: Implementar máquina de estados
 * - Percorrer string do início até 'pos'
 * - Rastrear estado atual (NONE, SINGLE, DOUBLE)
 * - Aspas dentro de outras aspas não mudam o estado
 * 
 * EXEMPLOS:
 * "echo 'hello world'"
 *       ^  (pos=6) -> QUOTE_SINGLE
 * 
 * "echo \"hello 'world'\""
 *              ^  (pos=13) -> QUOTE_DOUBLE (aspas simples ignoradas)
 * 
 * @param str String completa
 * @param pos Posição a verificar
 * @param state Ponteiro para armazenar o estado atual
 * @return TRUE se estiver quotado, FALSE caso contrário
 */
int	is_in_quotes(char *str, int pos, t_quote_state *state)
{
	// TODO: Implementar
	(void)str;
	(void)pos;
	(void)state;
	return (FALSE);
}

/**
 * @brief Remove aspas de uma string
 * 
 * TODO: Implementar remoção
 * - Percorrer string rastreando estado de quotes
 * - Copiar caracteres para nova string, exceto as aspas
 * - Manter o conteúdo dentro das aspas intacto
 * 
 * EXEMPLOS:
 * Input:  "hello"       -> Output: hello
 * Input:  'hello'       -> Output: hello
 * Input:  "hel'lo"      -> Output: hel'lo
 * Input:  'hel"lo'      -> Output: hel"lo
 * Input:  "hello" world -> Output: hello world
 * 
 * ATENÇÃO: Esta função é chamada DEPOIS da expansão!
 * 
 * @param str String com aspas
 * @return Nova string sem aspas (precisa dar free depois)
 */
char	*remove_quotes(char *str)
{
	// TODO: Implementar
	(void)str;
	return (NULL);
}

/**
 * @brief Verifica se há aspas não fechadas
 * 
 * TODO: Implementar validação
 * - Rastrear abertura e fechamento de aspas
 * - Retornar erro se houver aspas desbalanceadas
 * 
 * EXEMPLOS:
 * Input: "hello world"  -> OK
 * Input: 'hello world'  -> OK
 * Input: "hello world   -> ERRO (não fechado)
 * Input: 'hello" world' -> OK (aspas duplas dentro de simples)
 * 
 * @param str String a validar
 * @return TRUE se válido, FALSE se houver erro
 */
int	validate_quotes(char *str)
{
	// TODO: Implementar
	(void)str;
	return (TRUE);
}
