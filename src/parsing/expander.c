/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <maria-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:00:00 by mona              #+#    #+#             */
/*   Updated: 2026/02/25 17:50:38 by maria-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
** @brief Adiciona um caractere ao buffer dinamicamente.
**
** @param base Buffer atual (pode ser NULL)
** @param c Caractere a ser adicionado
** @return Novo buffer concatenado, ou NULL em caso de erro.
*/
static char	*append_char_to_buffer(char *base, char c)
{
	char	tmp[2];

	tmp[0] = c;
	tmp[1] = '\0';
	return (append_to_buffer(base, tmp));
}

/*
** @brief Processa e expande uma variável na string.
**
** Extrai o nome, expande o valor e avança o índice.
**
** @param str Ponteiro para o início do $VAR
** @param mini Estrutura principal
** @param i Ponteiro para índice a ser avançado
** @return String expandida (malloc), ou NULL
*/
static char	*expand_one_var(char *str, t_mini *mini, int *i)
{
	char	*var_name;
	char	*value;
	int		len;

	len = 0;
	var_name = extract_var_name(str + 1, &len);
	value = expand_var_value(var_name, mini);
	free(var_name);
	*i += len + 1;
	return (value);
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
 * @param str String originalnext->
 * @param mini Estrutura principal (para acessar env e exit_status)
 * @return Nova string com variáveis expandidas
 */
char	*expand_string(char *str, t_mini *mini)
{
	char	*result;
	int		i;
	char	*value;

	if (!str || !mini)
		return (NULL);
	if (is_single_quoted(str))
		return (ft_strdup(str));
	result = NULL;
	i = 0;
	while (str[i])
	{
		if (is_var_start(str[i], str[i + 1]))
		{
			value = expand_one_var(&str[i], mini, &i);
			result = append_to_buffer(result, value);
			free(value);
		}
		else
			result = append_char_to_buffer(result, str[i++]);
	}
	return (result);
}

/*
** @brief Concatena dinamicamente dois pedaços de string, realocando o buffer.
**
** Recebe um buffer base (pode ser NULL) e um pedaço a ser adicionado.
** Retorna um novo buffer concatenado e libera o antigo.
**
** @param base Buffer atual (pode ser NULL)
** @param add Pedaço a ser adicionado
** @return Novo buffer concatenado, ou NULL em caso de erro.
*/
static char	*append_to_buffer(char *base, const char *add)
{
	char	*newbuf;
	int		len_base;
	int		len_add;

	len_base = 0;
	len_add = 0;
	if (base)
		len_base = ft_strlen(base);
	if (add)
		len_add = ft_strlen(add);
	newbuf = ft_calloc(len_base + len_add + 1, sizeof(char));
	if (!newbuf)
	{
		free(base);
		return (NULL);
	}
	if (base)
		ft_strlcpy(newbuf, base, len_base + 1);
	if (add)
		ft_strlcat(newbuf, add, len_base + len_add + 1);
	free(base);
	return (newbuf);
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
	t_token	*curr;
	char	*expanded;

	if (!tokens || !mini)
		return ;
	curr = tokens;
	while (curr)
	{
		if (curr->type == TOKEN_WORD && !is_single_quoted(curr->value))
		{
			expanded = expand_string(curr->value, mini);
			free(curr->value);
			curr->value = expanded;
		}
		else if ((curr->type == TKN_REDIR_HEREDOC || curr->type == TKN_REDIR_OUT
				|| curr->type == TKN_REDIR_APPEND || curr->type == TKN_REDIR_IN)
			&& curr->next && curr->next->type == TOKEN_WORD
			&& !is_single_quoted(curr->next->value))
		{
			expanded = expand_string(curr->next->value, mini);
			free(curr->next->value);
			curr->next->value = expanded;
		}
		curr = curr->next;
	}
}
