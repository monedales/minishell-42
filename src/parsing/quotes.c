/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <maria-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/02/13 17:45:46 by maria-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_quote_state(char c, t_quote_state *state)
{
	if (c == '"' && *state == QUOTE_NONE)
		*state = QUOTE_DOUBLE;
	else if (c == '"' && *state == QUOTE_DOUBLE)
		*state = QUOTE_NONE;
	else if (c == '\'' && *state == QUOTE_NONE)
		*state = QUOTE_SINGLE;
	else if (c == '\'' && *state == QUOTE_SINGLE)
		*state = QUOTE_NONE;
}

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
	int	i;

	*state = QUOTE_NONE;
	i = 0;
	while (i < pos && str[i])
	{
		update_quote_state(str[i], state);
		i++;
	}
	return (*state != QUOTE_NONE);
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
	char			*result;
	t_quote_state	state;
	size_t			i;
	size_t			j;

	if (!str)
		return (NULL);
	result = malloc(sizeof(char) * ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	state = QUOTE_NONE;
	i = 0;
	j = 0;
	while (str[i])
	{
		if ((str[i] == '"' && (state == QUOTE_NONE || state == QUOTE_DOUBLE))
			|| (str[i] == '\'' && (state == QUOTE_NONE
					|| state == QUOTE_SINGLE)))
			update_quote_state(str[i], &state);
		else
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
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
 * @param str String to be validated
 * @return TRUE if valid, FALSE if there's a mistake
 */
int	validate_quotes(char *str)
{
	int				i;
	t_quote_state	state;

	state = QUOTE_NONE;
	i = 0;
	while (str[i])
	{
		update_quote_state(str[i], &state);
		i++;
	}
	if (state != QUOTE_NONE)
		return (FALSE);
	return (TRUE);
}
