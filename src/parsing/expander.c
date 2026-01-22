/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/01/21 20:21:33 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Verifica se um caractere é o início de uma variável
 * 
 * TODO: Implementar
 * - Retornar TRUE se for '$' seguido de letra, dígito ou '_'
 * - Caso especial: '$?' é sempre uma variável
 * 
 * @param c Caractere atual
 * @param next Próximo caractere
 * @return TRUE se for início de variável, FALSE caso contrário
 */
static int	is_var_start(char c, char next)
{
	// TODO: Implementar
	(void)c;
	(void)next;
	return (FALSE);
}

/**
 * @brief Extrai o nome da variável (sem o $)
 * 
 * TODO: Implementar extração
 * - Extrair caracteres válidos (letras, dígitos, underscore)
 * - Caso especial: "?" sozinho (para $?)
 * 
 * EXEMPLO:
 * Input: "$USER_HOME" -> "USER_HOME"
 * Input: "$?" -> "?"
 * 
 * @param str String começando após o '$'
 * @param len Ponteiro para armazenar o tamanho extraído
 * @return Nome da variável extraído
 */
static char	*extract_var_name(char *str, int *len)
{
	// TODO: Implementar
	(void)str;
	(void)len;
	return (NULL);
}

/**
 * @brief Expande uma variável em uma string
 * 
 * TODO: Implementar expansão
 * - Encontrar todas ocorrências de $VAR
 * - Substituir pelo valor da variável (get_env_value)
 * - Caso especial: $? deve ser substituído por last_exit_status
 * - IMPORTANTE: Não expandir dentro de aspas simples!
 * 
 * EXEMPLO:
 * Input: "Hello $USER" com USER=mona -> "Hello mona"
 * Input: "exit: $?" com status=127 -> "exit: 127"
 * Input: 'Hello $USER' -> 'Hello $USER' (sem expansão!)
 * 
 * @param str String original
 * @param mini Estrutura principal (para acessar env e exit_status)
 * @return Nova string com variáveis expandidas
 */
static char	*expand_string(char *str, t_mini *mini)
{
	// TODO: Implementar
	(void)str;
	(void)mini;
	return (NULL);
}

/**
 * @brief Verifica se a string está entre aspas simples
 * 
 * TODO: Implementar verificação
 * - Aspas simples desabilitam TODA expansão
 * - Aspas duplas permitem expansão
 * 
 * @param str String a verificar
 * @return TRUE se estiver em aspas simples, FALSE caso contrário
 */
static int	is_single_quoted(char *str)
{
	// TODO: Implementar
	(void)str;
	return (FALSE);
}

/**
 * @brief Expande variáveis em todos os tokens
 * 
 * ESTRATÉGIA:
 * 1. Percorrer lista de tokens
 * 2. Para cada TOKEN_WORD:
 *    - Verificar se não está em aspas simples
 *    - Expandir variáveis ($VAR, $?)
 *    - Substituir o value do token pela versão expandida
 * 3. Não tocar em tokens que não sejam WORD
 * 
 * ATENÇÃO:
 * - Tokens de redirecionamento não devem ser expandidos!
 * - Apenas o valor após o redirecionamento (nome do arquivo)
 * 
 * @param tokens Lista de tokens a expandir (modificada in-place)
 * @param mini Estrutura principal com env e exit_status
 */
void	expand_tokens(t_token *tokens, t_mini *mini)
{
	// TODO: Implementar
	(void)tokens;
	(void)mini;
}
