/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/02/20 21:54:40 by mona             ###   ########.fr       */
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
	if (c != '$')
		return (FALSE);
	if (next == '?')
		return (TRUE);
	if (ft_isalnum(next) == 1 || next == '_')
		return (TRUE);
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
	size_t	i;
	char	*s1;
	char	*var_name;
	
	if (!str || !len)
		return (NULL);
	i = 0;
	if (str[0] == '?')
	{
		s1 = ft_strdup("?");
		*len = 1;
		return (s1);
	}
	if (!is_var_start('$', str[0]))
		return (NULL);
	while(str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	*len = i;
	var_name = ft_calloc(*len + 1, sizeof(char));
	if (!var_name)
		return (NULL);
	ft_strlcpy(var_name, str, i + 1);
	return (var_name);
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
static int is_single_quoted(char *str)
{
	size_t          i;
	t_quote_state   state;

	if (!str)
		return (FALSE);
	i = 0;
	if (str[0] != '\'')
		return (FALSE);
	state = QUOTE_NONE;
	while (str[i])
	{
		update_quote_state(str[i], &state);
		i++;
	}
	if (state == QUOTE_NONE && str[i - 1] == '\'')
		return (TRUE);
	else
		return (FALSE);
}
/*
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
