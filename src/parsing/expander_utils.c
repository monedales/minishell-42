/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <maria-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 17:30:48 by maria-ol          #+#    #+#             */
/*   Updated: 2026/02/25 17:39:21 by maria-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
int	is_single_quoted(char *str)
{
	size_t			i;
	t_quote_state	state;

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
int	is_var_start(char c, char next)
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
char	*extract_var_name(char *str, int *len)
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
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	*len = i;
	var_name = ft_calloc(*len + 1, sizeof(char));
	if (!var_name)
		return (NULL);
	ft_strlcpy(var_name, str, i + 1);
	return (var_name);
}

/*
** @brief Expande uma variável e retorna o valor expandido.
**
** Se for $?, retorna o status. Caso contrário, busca no env.
**
** @param var_name Nome da variável
** @param mini Estrutura principal
** @return Valor expandido (mallocado), ou string vazia se não existir
*/
char	*expand_var_value(const char *var_name, t_mini *mini)
{
	char	*value;

	if (!ft_strncmp(var_name, "?", 2))
		return (ft_itoa(mini->last_exit_status));
	value = get_env_value(mini->env, var_name);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}
